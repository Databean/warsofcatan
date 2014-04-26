#include "GamePiece.h"

#include "GameBoard.h"
#include "GameVisitor.h"

/**
 * Initializes a GamePiece.
 * @param board The board the GamePiece is on.
 * @param location The location the GamePiece is on the board.
 */
GamePiece::GamePiece(GameBoard& board, Coordinate location) :
board(board), location(location){
	
}

/**
 * Base class destructor for GamePiece.
 */
GamePiece::~GamePiece() {
	
}

/**
 * The location of the GamePiece on the board.
 * @return Its location.
 */
Coordinate GamePiece::getCoordinates() const {
	return location;
}

/**
 * The board the GamePiece is on.
 * @return The board.
 */
GameBoard& GamePiece::getBoard() {
	return board;
}

/**
 * The const board the GamePiece is on.
 * @return The board.
 */
const GameBoard& GamePiece::getBoard() const {
	return board;
}

/**
 * The location of the GamePiece on the board.
 * @return The location.
 */
Coordinate GamePiece::getLocation() const {
	return location;
}

/**
 * Construct a resource tile.
 * @param board The GameBoard the resource tile is on.
 * @param location The place the resource tile is on the board.
 * @param resource The type of resource tile.
 * @param value The dice value required to make this tile pay out.
 */
ResourceTile::ResourceTile(GameBoard& board, Coordinate location, resourceType resource, int value) : 
GamePiece(board, location), resource(resource), value(value) {
	
}

/**
 * Destroy a resource tile.
 */
ResourceTile::~ResourceTile() {
	
}

/**
 * Visitor pattern double-dispatch method.
 * @param visitor The visiting instance.
 */
void ResourceTile::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

/**
 * The resource produced by this tile.
 * @return The type.
 */
resourceType ResourceTile::getType() const {
	return resource;
}

/**
 * The value required to be rolled to make this tile pay out.
 * @return The dice value.
 */
int ResourceTile::getDiceValue() const {
	return value;
}

/**
 * Equality comparison with another GamePiece.
 * @param other The GamePiece to compare this one with.
 * @return Whether the two pieces are equal.
 */
bool ResourceTile::operator==(const GamePiece& other) const {
	const ResourceTile* tile = dynamic_cast<const ResourceTile*>(&other);
	if(tile == nullptr) {
		return false;
	}
	
	return getType() == tile->getType() && getDiceValue() == tile->getDiceValue();
}

/**
 * Pay resource cards to the owners of adjacent settlements and cities.
 */
void ResourceTile::Payout() const{
	if (getBoard().getRobber() == location) //no need to pay out
		return;

	//std::vector<CornerPiece*> neighbors = getBoard().GetNeighboringCorners(location);
	//std::vector<CornerPiece*>::iterator it = neighbors.begin();

	std::vector<Settlement*> neighbors = getBoard().GetNeighboringSettlements(location);
	std::vector<Settlement*>::iterator it = neighbors.begin();
	while (it != neighbors.end())
	{
		(*it)->getOwner().addResource(resource, (*it)->getResourceModifier());
		std::cout << ((*it)->getOwner().getWood()) << "\n";
		std::cout << ((*it)->getOwner().getBrick()) << "\n";
		std::cout << ((*it)->getOwner().getOre()) << "\n";
		std::cout << ((*it)->getOwner().getWheat()) << "\n";
		std::cout << ((*it)->getOwner().getWool()) << "\n";

		it++;
	}

}

