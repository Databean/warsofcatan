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

class GameVisitor;

class GameBoard {
private:
	std::map<Coordinate, std::unique_ptr<CornerPiece>> corners;
	std::map<Coordinate, std::unique_ptr<GamePiece>> resources;
	std::map<Coordinate, std::vector<std::shared_ptr<Road>>> roads;
	std::vector<std::unique_ptr<Player>> players;
	
    void addResource(int x, int y, resourceType res, int val);
    bool checkRolls(int* rolls);
	
	bool isValidBoard() const;
	
	bool verifyRoadPlacement(Coordinate start, Coordinate end, Player& Owner);
	bool outOfBounds(const Coordinate& coord);
	bool roadExists(Coordinate start, Coordinate end);
	bool isRoadConnectionPoint(Coordinate point, Player& Owner);

	int constructBoardFromFile(std::ifstream &file);
	int constructFileFromBoard(std::ofstream &file);

	void removeRoadEnd(std::shared_ptr<Road> startRoad);
	int FindLongestRoad_FromPoint(Coordinate curr, Player & owner, std::map<Coordinate, bool>& marked, int length);
	
	void createRing(Coordinate topRight, int sideLength, std::vector<resourceType>& resources, std::vector<int>& rolls);
	void insertTile(Coordinate location, std::vector<resourceType>& resources, std::vector<int>& rolls);
public:
	GameBoard(std::vector<std::unique_ptr<Player>>&& players);
	GameBoard(std::vector<std::unique_ptr<Player>>&& players, const std::map<Coordinate, std::pair<resourceType, int>>& resourceLocations);
	GameBoard(std::istream& in);
	GameBoard(GameBoard&) = delete;
	~GameBoard();
	GameBoard& operator=(GameBoard&) = delete;
	
	void save(std::ostream& out);
	
	const std::map<Coordinate, std::unique_ptr<GamePiece>>& getResources() const;
	std::shared_ptr<Road> getRoad(Coordinate start, Coordinate end);
	
	int FindLongestRoad(Player & owner);

	std::vector<Settlement*> GetNeighboringSettlements(Coordinate location);

	bool buyRoad(Coordinate start, Coordinate end, Player& Owner);

	void PlaceSettlement(Coordinate location, Player& Owner);
	void PlaceCity(Coordinate location, Player& Owner);
	bool PlaceRoad(Coordinate start, Coordinate end, Player& Owner);
	
	void accept(GameVisitor& visitor);
	
	bool operator==(const GameBoard& other) const;
    
    bool testRollChecking(int* rolls);
};

#endif
