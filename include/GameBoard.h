#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <map>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>

#include "Util.h"
#include "GamePiece.h"



enum resourceType { WHEAT, SHEEP, STONE, BRICK, WOOD, DESERT };

class GameBoard {
private:
	std::map<Coordinate, std::unique_ptr<GamePiece>> corners;
	std::map<Coordinate, std::unique_ptr<GamePiece>> resources;
	std::vector<std::unique_ptr<const Road>> roads;
    
public:
	GameBoard();
	GameBoard(GameBoard&) = delete;
	~GameBoard();
	GameBoard& operator=(GameBoard&) = delete;
	std::vector<Settlement*> GetNeighboringSettlements(Coordinate location);

	void PlaceSettlement(Coordinate location, Player& Owner);

	void init_resources();
};





#endif