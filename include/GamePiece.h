#ifndef GAME_PIECE_H
#define GAME_PIECE_H

#include "Util.h"
#include "Player.h"

class GameBoard;

class GamePiece {
private:
	GameBoard& board;
public:
	GamePiece(GameBoard& board, Coordinate location);
	GamePiece(GamePiece&) = delete;
	virtual ~GamePiece();
	Coordinate location; 
	//virtual GamePiece& operator=(GamePiece&) = delete;
};

class ResourceTile : public GamePiece {
private:
	
public:
	ResourceTile(GameBoard& board);
	ResourceTile(GameBoard& board, Coordinate location, int resource, int value);
	ResourceTile(ResourceTile&) = delete;

	//dispense resource cards to owning players
	void Payout();

	int resource;
	int value;

	virtual ~ResourceTile();
	//virtual ResourceTile& operator=(ResourceTile&) = delete;
};

class Settlement : public GamePiece {
private:
	Player& owner;
public:
	Settlement(GameBoard& board, Coordinate location, Player& owner);
	Settlement(Settlement&) = delete;
	virtual ~Settlement();
	bool city;
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