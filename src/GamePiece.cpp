#include "GamePiece.h"

#include "GameBoard.h"
#include "GameVisitor.h"

GamePiece::GamePiece(GameBoard& board, Coordinate location) :
board(board), location(location){
	
}

GamePiece::~GamePiece() {
	
}

Coordinate GamePiece::getCoordinates() const {
	return location;
}

GameBoard& GamePiece::getBoard() {
	return board;
}

const GameBoard& GamePiece::getBoard() const {
	return board;
}

Coordinate GamePiece::getLocation() const {
	return location;
}

ResourceTile::ResourceTile(GameBoard& board, Coordinate location, resourceType resource, int value) : 
GamePiece(board, location), resource(resource), value(value) {
	
}

ResourceTile::~ResourceTile() {
	
}

void ResourceTile::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

resourceType ResourceTile::getType() const {
	return resource;
}

int ResourceTile::getDiceValue() const {
	return value;
}

bool ResourceTile::operator==(const GamePiece& other) const {
	const ResourceTile* tile = dynamic_cast<const ResourceTile*>(&other);
	if(tile == nullptr) {
		return false;
	}
	
	return getType() == tile->getType() && getDiceValue() == tile->getDiceValue();
}

//pay resource cards to owners of this tile

void ResourceTile::Payout() const{
	std::vector<CornerPiece*> neighbors = getBoard().GetNeighboringCorners(location);
	std::vector<CornerPiece*>::iterator it = neighbors.begin();
	while (it != neighbors.end())
	{
		(*it)->getOwner().addResource(resource, (*it)->getResourceModifier());
		it++;
	}
}

