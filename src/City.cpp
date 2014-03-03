#include "City.h"

#include "GameVisitor.h"

City::City(GameBoard& board, Coordinate location, Player& owner) : CornerPiece(board, location, owner) {
	
}

City::~City() {
	
}

void City::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

bool City::operator==(const GamePiece& p) const {
	return false;
}
