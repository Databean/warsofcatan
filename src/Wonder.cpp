/*
 * Wonder.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: Kyle Grage
 */

#include "Wonder.h"

#include "City.h"
#include "Settlement.h"
#include "GameVisitor.h"

/**
 * Construct a city.
 * @param board The GameBoard the city is a part of.
 * @param location The location of the city
 * @param owner The owner of the city
 */
Wonder::Wonder(GameBoard& board, Coordinate location, Player& owner) : CornerPiece(board, location, owner) {

}

/**
 * Destroy the city object.
 */
Wonder::~Wonder() {

}

/**
 *
 * @param visitor The visiting member.
 */
void Wonder::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

/**
 * Test for equality with another game piece.
 * @param p The piece to test with
 * @return True if this object is equal to the other, false if not.
 */
bool Wonder::operator==(const GamePiece& p) const {
	return false; //TODO: Actually compare.
}

/**
 * The number of resources this city gets from an adjacent tile when it pays out.
 * @return The number of resources acquired from one adjacent tile.
 */
int Wonder::getResourceModifier() {
	return 10;
}

/**
 * The victory points the city is worth.
 * @return The victory points the player gets from the city.
 */
int Wonder::getVictoryPoints() {
	return 10;
}

/**
 * Construct a Wonder from an existing CornerPiece (e.g. when upgrading a Settlement to a Wonder).
 * @param sett The CornerPiece to take initialization data from.
 */
Wonder::Wonder(CornerPiece& sett) : CornerPiece(sett.getBoard(), sett.getLocation(), sett.getOwner()) {

}

/**
 * Determines if a piece is the same
 * @return boolean
 */
bool City::operator==(const City& other) const {
	auto won = dynamic_cast<const Wonder*>(&other);
	if(won) {
		return getOwner().getName() == won->getOwner().getName() && getLocation() == won->getLocation();
	} else {
		return false;
	}
}

