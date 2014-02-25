#include "Settlement.h"

#include "GameVisitor.h"

Settlement::Settlement(GameBoard& board, Coordinate location, Player& owner) : CornerPiece(board, location, owner) {
	
}

Settlement::~Settlement() {
	
}

void Settlement::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}
