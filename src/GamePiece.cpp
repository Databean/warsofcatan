#include "GamePiece.h"

#include "GameBoard.h"
#include "GameVisitor.h"

GamePiece::GamePiece(GameBoard& board, Coordinate location) : board(board), location(location) {
	
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

ResourceTile::ResourceTile(GameBoard& board, Coordinate location, resourceType resource, int value) : 
GamePiece(board, location), resource(resource), value(value) {
	
}

ResourceTile::~ResourceTile() {
	
}

void ResourceTile::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

//pay resource cards to owners of this tile
/*
void ResourceTile::Payout() {
	std::vector<GamePiece> neighbors = board.GetNeighbors(location);
	for (int i = 0; i < neighbors.size; i++) //someone tell me how to traverse a vector less stupidly
	{
		neighbors[i].owner.addresource(resource, 1 + neighbors[i].city)
	}
}
*/
