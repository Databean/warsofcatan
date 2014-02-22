#include "GamePiece.h"

#include "GameBoard.h"

#include <stdexcept>

using std::runtime_error;

GamePiece::GamePiece(GameBoard& board) : board(board) {
	
}

GamePiece::~GamePiece() {
	
}

Coordinate GamePiece::getCoordinates() const {
	for(auto& it : board.getPieces()) {
		if(it.second.get() == this) {
			return it.first;
		}
	}
	throw runtime_error("This GamePiece does not exist on the board it holds a reference to.");
}

GameBoard& GamePiece::getBoard() {
	return board;
}

const GameBoard& GamePiece::getBoard() const {
	return board;
}

ResourceTile::ResourceTile(GameBoard& board, Type type, unsigned short diceValue) : GamePiece(board), type(type), diceValue(diceValue) {
	if(type != WOOD && type != SHEEP && type != ORE && type != BRICK && type != GRAIN && type != DESERT) {
		throw runtime_error("Invalid resource tile type");
	}
	if(diceValue < 2 || diceValue > 12) {
		throw runtime_error("Invalid dice value");
	}
}

ResourceTile::~ResourceTile() {
	
}

ResourceTile::Type ResourceTile::getType() const {
	return type;
}

unsigned short ResourceTile::getDiceValue() const {
	return diceValue;
}

Settlement::Settlement(GameBoard& board, Player& owner) : GamePiece(board), owner(owner) {
	
}

Settlement::~Settlement() {
	
}

Road::Road(GameBoard& board, Player& owner, Coordinate start, Coordinate end) : board(board), owner(owner), start(start), end(end) {
	
}

Road::Road(Road& other) : board(other.board), owner(other.owner), start(other.start), end(other.end) {
	
}

Road::~Road() {
	
}
