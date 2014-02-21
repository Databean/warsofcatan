#include "GameBoard.h"

#include <map>
#include <memory>

using std::map;
using std::unique_ptr;

GameBoard::GameBoard() {
	pieces.emplace(Coordinate(0, 0), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::WOOD, 2)));
	pieces.emplace(Coordinate(1, 1), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::SHEEP, 2)));
	pieces.emplace(Coordinate(-1, -1), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::BRICK, 2)));
	pieces.emplace(Coordinate(2, -1), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::ORE, 2)));
	pieces.emplace(Coordinate(-2, 1), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::DESERT, 2)));
	pieces.emplace(Coordinate(1, -2), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::GRAIN, 2)));
	pieces.emplace(Coordinate(-1, 2), unique_ptr<GamePiece>(new ResourceTile(*this, ResourceTile::BRICK, 2)));
}

GameBoard::~GameBoard() {
	
}

const map<Coordinate, unique_ptr<GamePiece>>& GameBoard::getPieces() const {
	return pieces;
}
