#include "CornerPiece.h"

CornerPiece::CornerPiece(GameBoard& board, Coordinate location, Player& owner) : GamePiece(board, location), owner(owner) {
	
}

CornerPiece::~CornerPiece() {
	
}

Player& CornerPiece::getOwner() {
	return owner;
}

const Player& CornerPiece::getOwner() const {
	return owner;
}
