#include "GamePiece.h"

#include "GameBoard.h"



GamePiece::GamePiece(GameBoard& board, Coordinate location) : board(board), location(location) {
	
}

GamePiece::~GamePiece() {
	
}

ResourceTile::ResourceTile(GameBoard& board, Coordinate location, int resource, int value) : 
GamePiece(board, location), resource(resource), value(value) {
	
}

ResourceTile::~ResourceTile() {
	int a;
}


//pay resource cards to owners of this tile

/*
void ResourceTile::Payout() {
	std::vector<GamePiece> neighbors;
	for (int i = 0; i < neighbors.size; i++) //someone tell me how to traverse a vector less stupidly
	{
		neighbors[i].owner.addresource(resource, 1 + neighbors[i].city)
	}
}
*/
Settlement::Settlement(GameBoard& board, Coordinate location, Player& owner) : 
GamePiece(board, location), owner(owner), city(0) {
	
}

Settlement::~Settlement() {
	
}

Road::Road(GameBoard& board, Player& owner, Coordinate start, Coordinate end) : board(board), owner(owner), start(start), end(end) {
	
}

Road::Road(Road& other) : board(other.board), owner(other.owner), start(other.start), end(other.end) {
	
}

Road::~Road() {
	
}
