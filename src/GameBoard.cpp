#include "GameBoard.h"


#include <map>
#include <memory>

using std::map;
using std::unique_ptr;

#define ADD_RESOURCE(x, y, res, val) (resources[Coordinate(x,y)] = \
std::unique_ptr<GamePiece>(new ResourceTile(*this, Coordinate(x,y), res, val)))
#define DUMMY_BOARD //define to instantiate dummy board for debugging 

GameBoard::GameBoard() {
	init_resources();
}

GameBoard::~GameBoard() {
	
}

int GameBoard::saveBoardToFile(std::string filename){
	std::ofstream file;
	file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try {
		filename = filename + ".wocs";
		file.open(filename.c_str(), std::fstream::out | std::fstream::trunc);
		constructFileFromBoard(file);

	} catch (std::ofstream::failure e){
		std::cerr << "Exception opening/reading/closing file \n";
	}
	return 0;
}

int GameBoard::loadBoardFromFile(std::string filename){
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		filename = filename + ".wocs";
		file.open(filename.c_str(), std::fstream::in);
		constructBoardFromFile(file);

		file.close();
	} catch (std::ifstream::failure e) {
		std::cerr << "Exception opening/reading/closing file\n";
	}
	return 0;
}

int GameBoard::constructBoardFromFile(std::ifstream &file){
	//Parse and construct the board from the file
	//@ TODO
	while (!file.eof()) {
		std::cout << file.get();
	}
	return 0;
}


int GameBoard::constructFileFromBoard(std::ofstream &file){
	//Construct the file based on the structure of the board
	//@ TODO
	file << "Hello World!";
	return 0;
}

const map<Coordinate, unique_ptr<GamePiece>>& GameBoard::getResources() const {
	return resources;
}

std::vector<Settlement*> GameBoard::GetNeighboringSettlements(Coordinate location) {
	static Coordinate adjacentCoordDiffs[] = {Coordinate(0, 1), Coordinate(1, 0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0), Coordinate(-1, 1)};
	std::vector<Settlement*> v;
	for(unsigned int i = 0; i < 6; i++) {
		const Coordinate& diff = adjacentCoordDiffs[i];
		Coordinate adjacentPoint(location.first + diff.first, location.second + diff.second);
		auto it = resources.find(adjacentPoint);
		if(it != resources.end()) {
			GamePiece* piece = it->second.get();
			if(dynamic_cast<Settlement*>(piece)) {
				v.push_back(static_cast<Settlement*>(piece));
			}
		}
	}
	return v;
}

/* initialize board with a set of resources. Right now, just creates a static tile arrangement to test
URL: http://images.fanpop.com/images/image_uploads/Differents-Boards-settlers-of-catan-521934_1157_768.jpg*/

void GameBoard::init_resources()
{
	

	#ifdef DUMMY_BOARD
	ADD_RESOURCE(0, 1, BRICK, 2);
	ADD_RESOURCE(-2, 2, SHEEP, 5);
	ADD_RESOURCE(2, 0, WOOD, 6);
	ADD_RESOURCE(-3, 3, DESERT, 0);
	ADD_RESOURCE(-1, 3, SHEEP, 10);
	ADD_RESOURCE(1, 2, WHEAT, 9);
	ADD_RESOURCE(3, 1, WHEAT, 3);
	ADD_RESOURCE(-4, 6, WOOD, 8);
	ADD_RESOURCE(-2, 5, SHEEP, 3);
	ADD_RESOURCE(0, 4, STONE, 11);
	ADD_RESOURCE(2, 3, STONE, 4);
	ADD_RESOURCE(4, 2, SHEEP, 8);
	ADD_RESOURCE(-3, 7, BRICK, 4);
	ADD_RESOURCE(-1, 6, WHEAT, 6);
	ADD_RESOURCE(1, 5, WOOD, 5);
	ADD_RESOURCE(3, 4, BRICK, 10);
	ADD_RESOURCE(-2, 8, WOOD, 11);
	ADD_RESOURCE(0, 7, STONE, 12);
	ADD_RESOURCE(2, 6, WHEAT, 9);
	#endif
	

}

void GameBoard::PlaceSettlement(Coordinate location, Player& Owner){
	corners[location] = std::unique_ptr<GamePiece>(new Settlement(*this, location, Owner));
}

