#include "GamePiece.h"

#include "GameBoard.h"

GamePiece::GamePiece(GameBoard& board) : board(board) {
	
}

GamePiece::~GamePiece() {
	
}

ResourceTile::ResourceTile(GameBoard& board) : GamePiece(board) {
	
}

ResourceTile::~ResourceTile() {
	
}

Settlement::Settlement(GameBoard& board, Player& owner) : GamePiece(board), owner(owner) {
	
}

Settlement::~Settlement() {
	
}

Road::Road(GameBoard& board, Player& owner, Coordinate start, Coordinate end) : board(board), owner(owner), start(start), end(end) {
	
}

Road::Road(Road& other) : board(other.board), owner(other.owner), start(other.start), end(other.end) {
	
}

Road::~Road() {
	
}
