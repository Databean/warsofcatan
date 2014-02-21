#include "GameBoard.h"

#include <map>
#include <memory>

using std::map;
using std::unique_ptr;

GameBoard::GameBoard() {
	
}

GameBoard::~GameBoard() {
	
}

const map<Coordinate, unique_ptr<GamePiece>>& GameBoard::getPieces() const {
	return pieces;
}
