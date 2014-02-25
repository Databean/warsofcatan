#ifndef GAME_PIECE_H
#define GAME_PIECE_H

#include "Util.h"
#include "Player.h"

enum resourceType { WHEAT, SHEEP, STONE, BRICK, WOOD, DESERT };

class GameBoard;

class GamePiece {
private:
	GameBoard& board;
public:
	GamePiece(GameBoard& board, Coordinate location);
	GamePiece(GamePiece&) = delete;
	virtual ~GamePiece();
	//virtual GamePiece& operator=(GamePiece&) = delete;
	
	Coordinate getCoordinates() const;
	GameBoard& getBoard();
	const GameBoard& getBoard() const;
	
	Coordinate location; 
};

class ResourceTile : public GamePiece {
public:
	ResourceTile(GameBoard& board);
	ResourceTile(GameBoard& board, Coordinate location, resourceType resource, int value);
	ResourceTile(ResourceTile&) = delete;
	//virtual ResourceTile& operator=(ResourceTile&) = delete;

	//dispense resource cards to owning players
	void Payout();

	resourceType resource;
	int value;

	virtual ~ResourceTile();
};

#endif

