#include "CornerPiece.h"

/**
 * Construct a CornerPiece with its board, location, and owner.
 * @param board The GameBoard the CornerPiece is on.
 * @param location The location the CornerPiece is on the GameBoard.
 * @param owner The owner of the CornerPiece.
 */
CornerPiece::CornerPiece(GameBoard& board, Coordinate location, Player& owner) : GamePiece(board, location), owner(owner) {
	
}

/**
 * Destroy the corner piece.
 */
CornerPiece::~CornerPiece() {
	
}

/**
 * Visitor double-dispatch method.
 * @param visitor The visiting instance.
 */
void CornerPiece::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

/**
 * Getter for the owner of the piece.
 * @return The piece's owner.
 */
Player& CornerPiece::getOwner() {
	return owner;
}

/**
 * Const getter for the owner of the piece.
 * @return The piece's owner.
 */
const Player& CornerPiece::getOwner() const {
	return owner;
}

/**
 * Getter for the victory points acquired from the piece.
 * @return The victory points this piece is worth.
 */
int CornerPiece::getVictoryPoints() {
	return 0;
}

/**
 * Getter for the resource acquired from adjacent resource tiles.
 * @return The resources this acquires from an adjacent resource tile.
 */
int CornerPiece::getResourceModifier() {
	return 0;
}

/**
 * Determines if a piece is the same
 * @return boolean
 */
bool CornerPiece::operator==(const GamePiece& other) const {
	auto cp = dynamic_cast<const CornerPiece*>(&other);
	if(cp) {
		return getOwner().getName() == cp->getOwner().getName() && getLocation() == cp->getLocation();
	} else {
		return false;
	}
}
