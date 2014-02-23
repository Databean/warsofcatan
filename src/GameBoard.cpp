#include "GameBoard.h"

#include <map>
#include <memory>

using std::map;
using std::unique_ptr;

GameBoard::GameBoard() {
	pieces.emplace(Coordinate(0, 0), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::WOOD, 2)));
	pieces.emplace(Coordinate(1, 1), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::SHEEP, 2)));
	pieces.emplace(Coordinate(-1, -1), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::BRICK, 2)));
	pieces.emplace(Coordinate(2, -1), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::ORE, 2)));
	pieces.emplace(Coordinate(-2, 1), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::DESERT, 2)));
	pieces.emplace(Coordinate(1, -2), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::GRAIN, 2)));
	pieces.emplace(Coordinate(-1, 2), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::BRICK, 2)));
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

const map<Coordinate, unique_ptr<GamePiece>>& GameBoard::getPieces() const {
	return pieces;
}
