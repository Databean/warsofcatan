#include "City.h"
#include "Settlement.h"
#include "GameVisitor.h"

/**
 * Construct a city.
 * @param board The GameBoard the city is a part of.
 * @param location The location of the city
 * @param owner The owner of the city
 */
City::City(GameBoard& board, Coordinate location, Player& owner) : CornerPiece(board, location, owner) {
	
}

/**
 * Destroy the city object.
 */
City::~City() {
	
}

/**
 * @param visitor The visiting member.
 */
void City::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

/**
 * Test for equality with another game piece.
 * @param p The piece to test with
 * @return True if this object is equal to the other, false if not.
 */
bool City::operator==(const GamePiece& p) const {
	return false; //TODO: Actually compare.
}

/**
 * The number of resources this city gets from an adjacent tile when it pays out.
 * @return The number of resources acquired from one adjacent tile.
 */
int City::getResourceModifier() {
	return 2;
}

/**
 * The victory points the city is worth.
 * @return The victory points the player gets from the city.
 */
int City::getVictoryPoints() {
	return 2;
}

/**
 * Construct a City from an existing CornerPiece (e.g. when upgrading a Settlement to a City).
 * @param sett The CornerPiece to take initialization data from.
 */
City::City(CornerPiece& sett) : CornerPiece(sett.getBoard(), sett.getLocation(), sett.getOwner()) {

}

/**
 * Determines if a piece is the same
 * @return boolean
 */
bool City::operator==(const City& other) const {
	auto city = dynamic_cast<const City*>(&other);
	if(city) {
		return getOwner().getName() == city->getOwner().getName() && getLocation() == city->getLocation();
	} else {
		return false;
	}
}
