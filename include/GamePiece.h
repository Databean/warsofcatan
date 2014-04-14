#ifndef GAME_PIECE_H
#define GAME_PIECE_H

#include "Util.h"
#include "Player.h"

class GameVisitor;

enum resourceType { WHEAT, SHEEP, STONE, BRICK, WOOD, DESERT };

class GameBoard;

/**
 * Something that occupies a position in triangular coordinates on the board. Currently is either a
 * CornerPiece(Settlement or City) or a ResourceTile.
 */
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
	
	virtual void accept(GameVisitor&) = 0;
	virtual bool operator==(const GamePiece& other) const = 0;
	
	Coordinate getLocation() const;

};

/**
 * One of the hexagons that has a particular resource and a dice number on the board. When the dice
 * roll its number, resources are paid out to adjacent cities.
 */
class ResourceTile : public GamePiece {
public:
	ResourceTile(GameBoard& board);
	ResourceTile(GameBoard& board, Coordinate location, resourceType resource, int value);
	ResourceTile(ResourceTile&) = delete;
	//virtual ResourceTile& operator=(ResourceTile&) = delete;

	//dispense resource cards to owning players
	void Payout() const;

	resourceType resource;
	int value;

	virtual ~ResourceTile();
	
	virtual void accept(GameVisitor&);
	virtual bool operator==(const GamePiece& other) const;
	
	resourceType getType() const;
	int getDiceValue() const;
};

#endif

