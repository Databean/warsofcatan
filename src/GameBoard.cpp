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
	freeRoads();
}

/*
 * Frees the roads data structure to prevent memory leaks
 */

void GameBoard::freeRoads(){
	//Iterate over all the points in the roads map
	for (auto roadVector = roads.begin(); roadVector != roads.end(); ++roadVector)
	{
		//Iterate all the roads at a given point
		for (std::vector<Road*>::iterator road = roadVector->second.begin(); road != roadVector->second.end(); ++road) {
			Road * roadPtr = *road;

			//If this is the start of the road we want to remove it, but we must first erase it the list at the other end of the road
			//If we don't then we may try to access a road which has already been freed
			if(roadPtr != NULL && roadPtr->getStart() == roadVector->first){
				removeRoadEnd(roadPtr);
				roadVector->second.erase(road);
				//Need to decrement the iterator to account for the lost item
				road--;
				delete roadPtr;
			}
		}
	}
}

/**
 * Find and remove the road that matches startRoad
 */
void GameBoard::removeRoadEnd(Road * startRoad){
	std::vector<Road*> endRoadVector = roads[startRoad->getEnd()];
	for(std::vector<Road*>::iterator endRoad = endRoadVector.begin(); endRoad != endRoadVector.end(); ++endRoad){
		if((*endRoad) == startRoad){
			endRoadVector.erase(endRoad);
			//Need to decrement the iterator to account for the lost item
			endRoad--;
		}
	}
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
	 *
	 * Discussed that we can just do a find in the map, and if it's not found then it's out of bounds
	 */

	switch (coord.second) {
	case 0:
		return !(coord.first >= 0 && coord.first <= 4);
		break;
	case 1:
		return !(coord.first >= -2 && coord.first <= 5);
		break;
	case 2:
		return !(coord.first >= -3 && coord.first <= 5);
		break;
	case 3:
		return !(coord.first >= -3 && coord.first <= 4);
		break;
	case 4:
		return !(coord.first >= -4 && coord.first <= 4);
		break;
	case 5:
		return !(coord.first >= -4 && coord.first <= 3);
		break;
	case 6:
		return !(coord.first >= -5 && coord.first <= 3);
		break;
	case 7:
		return !(coord.first >= -5 && coord.first <= 2);
		break;
	case 8:
		return !(coord.first >= -4 && coord.first <= 0);
		break;
	default:
		break;
	}
	return true;
}

/**
 * Checks to make sure the road doesn't already exist. If it does, then we don't want to add it again
 */
bool GameBoard::roadExists(Coordinate start, Coordinate end) {
	Road * isRoad = getRoad(start, end);
	if (isRoad == NULL)
		return false;
	return true;
}

/**
 * Checks to make sure the road being placed at a valid point according to the rules
 */
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

/**
 * Runs a series of checks to make sure the road can be placed
 */
bool GameBoard::verifyRoadPlacement(Coordinate start, Coordinate end, Player& Owner) {
	if (outOfBounds(start) || outOfBounds(end))
		return false;

	if (roadExists(start, end))
		return false;

	if (!isRoadConnectionPoint(start, end, Owner))
		return false;

	return true;
}

/**
 * Places a road at the specified coordinates that will be owned by the given player
 */
void GameBoard::PlaceRoad(Coordinate start, Coordinate end, Player& Owner) {
	if (!verifyRoadPlacement(start, end, Owner))
		return;

	Road * newRoad;
	try {
		newRoad = new Road(start, end, Owner);
	} catch (int n) {
		//Coordinates did not meet the criteria for a valid road
		return;
	}
	std::vector<Road*> roadVector = roads[start];
	roadVector.push_back(newRoad);
	roads[start] = roadVector;

	roadVector = roads[end];
	roadVector.push_back(newRoad);
	roads[end] = roadVector;
}

/**
 * returns a pointer to the road located at the specified coordinates. Will return NULL if the road is not found
 */
Road * GameBoard::getRoad(Coordinate start, Coordinate end){
	std::vector<Road*> roadVector = roads[start];
	for (std::vector<Road*>::iterator road = roadVector.begin(); road != roadVector.end(); ++road) {
		if ((*road)->equals(start, end))
			return *road;
	}
	return NULL;
}

/**
 * Parent function for the find longest road traversal. Note that longest path is NP-Hard, so there is no simple algorithm for this.
 */
int GameBoard::FindLongestRoad(Player & owner){
	int longest_path = 0;
	//for each road vertex v on the board
	for (auto roadVector = roads.begin(); roadVector != roads.end(); ++roadVector){
		//find the longest path from v
		std::map<Coordinate, bool> marked;
		Coordinate start = roadVector->first;
		int temp_longest_path = FindLongestRoad_FromPoint(start, owner, marked, 0);

		std::cout << "LONGEST PATH: " << start.first << ", " << start.second << ": " << temp_longest_path << "\n";

		//if that path is longer than the current longest, set to the longest
		if (temp_longest_path > longest_path)
			longest_path = temp_longest_path;
	}

	return longest_path;
}


int GameBoard::FindLongestRoad_FromPoint(Coordinate curr, Player & owner, std::map<Coordinate, bool>& marked, int length){

	std::cout << "       " << curr.first << ", " << curr.second << ": " << length << "\n";

	marked[curr] = true;
	int longest_path = length;
	//traverse all the surrounding edges and vertices
	std::vector<Road*> roadVector = roads[curr];
	for (std::vector<Road*>::iterator road = roadVector.begin(); road != roadVector.end(); ++road) {

		int temp_longest_path = length;

		//if the owner is correct and the road is unmarked
		if ( !(*road)->isMarked() && (*road)->owner->getName().compare(owner.getName()) == 0){

			temp_longest_path++;
			(*road)->mark();
			//Check if you can traverse to the next vertex and make that step if you can
			if(curr != (*road)->getStart() && !marked[(*road)->getStart()]){
				temp_longest_path = FindLongestRoad_FromPoint((*road)->getStart(), owner, marked, temp_longest_path);
			}else if (curr != (*road)->getEnd() && !marked[(*road)->getEnd()]){
				temp_longest_path = FindLongestRoad_FromPoint((*road)->getEnd(), owner, marked, temp_longest_path);
			}
			(*road)->unmark();
		}

		if(temp_longest_path > longest_path)
			longest_path = temp_longest_path;
	}
	marked[curr] = false;
	return longest_path;
}















