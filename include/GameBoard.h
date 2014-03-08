#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <map>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include <cstddef>

#include "Util.h"
#include "GamePiece.h"
#include "Settlement.h"
#include "tinyxml2.h"
#include "Road.h"

class GameBoard {
private:
	std::map<Coordinate, std::unique_ptr<GamePiece>> corners;
	std::map<Coordinate, std::unique_ptr<GamePiece>> resources;

	std::map<Coordinate, std::vector<Road*>> roads;
	
	bool verifyRoadPlacement(Coordinate start, Coordinate end, Player& Owner);
	bool outOfBounds(const Coordinate& coord);
	bool roadExists(Coordinate start, Coordinate end);
	bool isRoadConnectionPoint(Coordinate start, Coordinate end, Player& Owner);

	int constructBoardFromFile(std::ifstream &file);
	int constructFileFromBoard(std::ofstream &file);

	void freeRoads();
	void removeRoadEnd(Road * startRoad);
	int FindLongestRoad_FromPoint(Coordinate curr, Player & owner, std::map<Coordinate, bool>& marked, int length);

public:
	GameBoard();
	GameBoard(GameBoard&) = delete;
	~GameBoard();
	GameBoard& operator=(GameBoard&) = delete;
	
	int save_Board(std::string filename);
	int load_Board(std::string filename);
	const std::map<Coordinate, std::unique_ptr<GamePiece>>& getResources() const;
	Road * getRoad(Coordinate start, Coordinate end);
	
	int FindLongestRoad(Player & owner);

	std::vector<Settlement*> GetNeighboringSettlements(Coordinate location);

	void PlaceSettlement(Coordinate location, Player& Owner);
	void PlaceRoad(Coordinate start, Coordinate end, Player& Owner);


	void init_resources();
};

#endif
