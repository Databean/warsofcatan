#include "Settlement.h"

#include "GameVisitor.h"

Settlement::Settlement(GameBoard& board, Coordinate location, Player& owner) : CornerPiece(board, location, owner) {
	
}

Settlement::~Settlement() {
	
}

void Settlement::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}



int Settlement::getVictoryPoints() {
	return 1;
}

int Settlement::getResourceModifier() {
	//return (board.getRobber() == location); projected implementation of 
	return 1;
	
}


bool Settlement::operator==(const GamePiece& other) const {
	auto settlement = dynamic_cast<const Settlement*>(&other);
	if(settlement) {
		return getOwner().getName() == settlement->getOwner().getName() && getLocation() == settlement->getLocation();
	} else {
		return false;
	}
}

