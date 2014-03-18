#include "City.h"
#include "Settlement.h"
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

int City::getResourceModifier() {
	return 2;
}

int City::getVictoryPoints() {
	return 2; //TODO: implement robber check here
}

City::City(CornerPiece& sett) : CornerPiece(sett.getBoard(), sett.getLocation(), sett.getOwner()) {

}
