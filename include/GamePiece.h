#ifndef GAME_PIECE_H
#define GAME_PIECE_H

#include "Util.h"
#include "Player.h"

class GameBoard;

class GamePiece {
private:
	GameBoard& board;
public:
	GamePiece(GameBoard& board);
	GamePiece(GamePiece&) = delete;
	virtual ~GamePiece();
	//virtual GamePiece& operator=(GamePiece&) = delete;
	
	Coordinate getCoordinates() const;
	GameBoard& getBoard();
	const GameBoard& getBoard() const;
};

class ResourceTile : public GamePiece {
public:
	enum Type { WOOD, SHEEP, ORE, BRICK, GRAIN, DESERT };
private:
	Type type;
	unsigned short diceValue;
public:
	ResourceTile(GameBoard& board, Type type, unsigned short diceValue);
	ResourceTile(ResourceTile&) = delete;
	virtual ~ResourceTile();
	//virtual ResourceTile& operator=(ResourceTile&) = delete;
	
	Type getType() const;
	unsigned short getDiceValue() const;
};

class Settlement : public GamePiece {
private:
	Player& owner;
public:
	Settlement(GameBoard& board, Player& owner);
	Settlement(Settlement&) = delete;
	virtual ~Settlement();
	//virtual Settlement& operator=(Settlement&) = delete;
};

class Road {
private:
	GameBoard& board;
	Player& owner;
	Coordinate start;
	Coordinate end;
public:
	Road(GameBoard& board, Player& owner, Coordinate start, Coordinate end);
	Road(Road&);
	~Road();
	//Road& operator=(Road&) = delete;
};

#endif

