#include "GameBoard.h"

#include <map>
#include <memory>

using std::map;
using std::unique_ptr;

#include <ctime>
#include <algorithm>

#define ADD_RESOURCE(x, y, res, val) (this->resources[Coordinate(x,y)] = \
std::unique_ptr<GamePiece>(new ResourceTile(*this, Coordinate(x,y), res, val)))
#define DUMMY_BOARD //define to instantiate dummy board for debugging 

using std::random_shuffle;
using std::time;

GameBoard::GameBoard() {
	init_resources();
}

GameBoard::~GameBoard() {

}

int GameBoard::save_Board(std::string filename) {
	std::ofstream file;
	file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try {
		filename = filename + ".wocs";
		file.open(filename.c_str());
		constructFileFromBoard(file);
		file.close();
		return 0;
	} catch (std::ofstream::failure e) {
		std::cerr << "Exception opening/closing/writing file: " << e.what();
	}
	return -1;
}

int GameBoard::load_Board(std::string filename) {
	std::ifstream file;
	try {
		filename = filename + ".wocs";
		file.open(filename.c_str());
		constructBoardFromFile(file);
		file.close();
		return 0;
	} catch (std::ifstream::failure e) {
		std::cerr << "Exception opening/closing/reading file: " << e.what();
	}
	return -1;
}

int GameBoard::constructBoardFromFile(std::ifstream &file) {
	//Parse and construct the board from the file
	//@ TODO
	std::string line;
	if (file.is_open()) {
		while (getline(file, line)) {
			std::cout << line << '\n';
		}
	}
	return 0;
}

int GameBoard::constructFileFromBoard(std::ofstream &file) {
	//Construct the file based on the structure of the board
	//@ TODO
	file << "Hello World!";
	return 0;
}

const map<Coordinate, unique_ptr<GamePiece>>& GameBoard::getResources() const {
	return resources;
}

std::vector<Settlement*> GameBoard::GetNeighboringSettlements(
		Coordinate location) {
	static Coordinate adjacentCoordDiffs[] = { Coordinate(0, 1), Coordinate(1,
			0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0),
			Coordinate(-1, 1) };
	std::vector<Settlement*> v;
	for (unsigned int i = 0; i < 6; i++) {
		const Coordinate& diff = adjacentCoordDiffs[i];
		Coordinate adjacentPoint(location.first + diff.first,
				location.second + diff.second);
		auto it = resources.find(adjacentPoint);
		if (it != resources.end()) {
			GamePiece* piece = it->second.get();
			if (dynamic_cast<Settlement*>(piece)) {
				v.push_back(static_cast<Settlement*>(piece));
			}
		}
	}
	return v;
}

/* initialize board with a set of resources. Currently only the standard configuration (no custom shapes or expansion packs) is implemented.  Board tiles and roll numbers are randomized.
 @todo Change the dummy board to the actual board
 */

void GameBoard::init_resources() {
	std::srand(std::time(0));

	resourceType resources[] = { BRICK, BRICK, BRICK, STONE, STONE, STONE,
			WHEAT, WHEAT, WHEAT, WHEAT, WOOD, WOOD, WOOD, WOOD, SHEEP, SHEEP,
			SHEEP, SHEEP, DESERT };
	random_shuffle(&resources[0], &resources[19]);

	int rolls[] = { 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11 };
	random_shuffle(&rolls[0], &rolls[18]);

	int xcoords[] = { 0, -2, 2, -3, -1, 1, 3, -4, -2, 0, 2, 4, -3, -1, 1, 3, -2,
			0, 2 };
	int ycoords[] = { 1, 2, 0, 4, 3, 2, 1, 6, 5, 4, 3, 2, 7, 6, 5, 4, 8, 7, 6 };

#ifdef DUMMY_BOARD
	int rollCount = 0;
	for (int i = 0; i < 19; i++) {
		if (resources[i] == DESERT) {
			ADD_RESOURCE(xcoords[i], ycoords[i], resources[i], 0);
		} else {
			ADD_RESOURCE(xcoords[i], ycoords[i], resources[i],
					rolls[rollCount]);
			rollCount++;
		}
	}
#endif
}

void GameBoard::PlaceSettlement(Coordinate location, Player& Owner) {
	corners[location] = std::unique_ptr < GamePiece
			> (new Settlement(*this, location, Owner));
}

/**
 * Checks to make sure the coordinate is within bounds of the board
 */
bool GameBoard::outOfBounds(const Coordinate& coord) {
	/**
	 * This code is embarrassing, but I couldn't really figure out how to easily check for out of bounds
	 * I'm sure there is a simple algebraic function that does it, but I went for the hacky way.
	 */
	switch (coord.second) {
	case 0:
		return (coord.first >= 0 && coord.first <= 4);
		break;
	case 1:
		return (coord.first >= -2 && coord.first <= 5);
		break;
	case 2:
		return (coord.first >= -3 && coord.first <= 5);
		break;
	case 3:
		return (coord.first >= -3 && coord.first <= 4);
		break;
	case 4:
		return (coord.first >= -4 && coord.first <= 4);
		break;
	case 5:
		return (coord.first >= -4 && coord.first <= 3);
		break;
	case 6:
		return (coord.first >= -5 && coord.first <= 3);
		break;
	case 7:
		return (coord.first >= -5 && coord.first <= 2);
		break;
	case 8:
		return (coord.first >= -4 && coord.first <= 0);
		break;
	default:
		break;
	}
	return false;
}

bool GameBoard::roadExists(Coordinate start, Coordinate end) {
	std::vector<Road*> roadVector = roads[start];
	for (std::vector<Road*>::iterator road = roadVector.begin();
			road != roadVector.end(); ++road) {
		if ((*road)->equals(start, end))
			return true;
	}
	return false;
}

bool GameBoard::isRoadConnectionPoint(Coordinate start, Coordinate end, Player& Owner){
	/** Need to figure out the CornerPiece/GamePiece predicament
	CornerPiece * corner = corners[start];
	if(corner != NULL){
		if (corner->getOwner() == Owner)
			return true;
	}
	return false;
	**/
	return true;
}

bool GameBoard::verifyRoadPlacement(Coordinate start, Coordinate end, Player& Owner) {
	if (outOfBounds(start) || outOfBounds(end))
		return false;

	if (roadExists(start, end))
		return false;

	if (!isRoadConnectionPoint(start, end, Owner))
		return false;
	return true;
}

void GameBoard::PlaceRoad(Coordinate start, Coordinate end, Player& Owner) {
	verifyRoadPlacement(start, end, Owner);
	Road * newRoad;
	try {
		newRoad = new Road(start, end, Owner);
	} catch (int n) {
		//Coordinates did not meet the criteria for a valid road
		return;
	}

	std::vector<Road*> roadVector = roads[start];
	roadVector.push_back(newRoad);

	roadVector = roads[end];
	roadVector.push_back(newRoad);
}

