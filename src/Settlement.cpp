#include "Settlement.h"

#include "GameVisitor.h"

Settlement::Settlement(GameBoard& board, Coordinate location, Player& owner) : CornerPiece(board, location, owner) {
	
}

Settlement::~Settlement() {
	
}

void Settlement::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

bool Settlement::operator==(const GamePiece&) const {
	return false;
}

int Settlement::getVictoryPoints() {
	return 1;
}

int Settlement::getResourceModifier() {
	//return (board.robber.location == location); projected implementation of 
	return 1;
}

