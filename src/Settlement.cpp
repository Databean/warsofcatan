#include "Settlement.h"

#include "GameVisitor.h"

/**
 * Construct a settlement.
 * @param board The GameBoard the settlement is on.
 * @param location The location the settlement is on the board.
 * @param owner The owner of the settlement.
 */
Settlement::Settlement(GameBoard& board, Coordinate location, Player& owner) : CornerPiece(board, location, owner) {
	
}

/**
 * Destroy a settlement.
 */
Settlement::~Settlement() {
	
}

/**
 * Visitor double-dispatch method.
 * @param visitor The visiting instance.
 */
void Settlement::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

/**
 * The victory points this settlement is worth.
 * @return The victory points.
 */
int Settlement::getVictoryPoints() {
	return 1;
}

/**
 * The resources players get from adjacent resource tiles each.
 * @return The resources acquired.
 */
int Settlement::getResourceModifier() {
	//return (board.getRobber() == location); projected implementation of 
	return 1;
	
}

/**
 * Compare the settlement for equality with another GamePiece.
 * @param other The GamePiece to test equality with.
 * @return Whether the two are equal.
 */
bool Settlement::operator==(const GamePiece& other) const {
	auto settlement = dynamic_cast<const Settlement*>(&other);
	if(settlement) {
		return getOwner().getName() == settlement->getOwner().getName() && getLocation() == settlement->getLocation();
	} else {
		return false;
	}
}

