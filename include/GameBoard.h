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

	std::map<Coordinate, std::unique_ptr<ResourceTile>> resources;


	std::map<Coordinate, std::vector<std::shared_ptr<Road>>> roads;

	std::vector<std::unique_ptr<Player>> players;
	Coordinate robber;

	
    void addResource(int x, int y, resourceType res, int val);
    bool checkRolls(int* rolls);
	
	bool isValidBoard() const;
	
	bool verifyRoadPlacement(Coordinate start, Coordinate end, Player& Owner) const;
	bool outOfBounds(const Coordinate& coord) const;
	bool roadExists(Coordinate start, Coordinate end) const;
	bool isRoadConnectionPoint(Coordinate point, Player& Owner) const;

	int constructBoardFromFile(std::ifstream &file);
	int constructFileFromBoard(std::ofstream &file);

	void removeRoadEnd(std::shared_ptr<Road> startRoad);
	int FindLongestRoad_FromPoint(Coordinate curr, const Player & owner, std::map<Coordinate, bool>& marked, std::map<Road*, bool>& markedRoads, int length) const;
	
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
	

	const std::map<Coordinate, std::unique_ptr<ResourceTile>>& getResources() const;



	const std::shared_ptr<Road> getRoad(Coordinate start, Coordinate end) const;

	
	int FindLongestRoad(const Player & owner) const;

	std::vector<Settlement*> GetNeighboringSettlements(Coordinate location) const;
	std::vector<CornerPiece*> GetNeighboringCorners(Coordinate location) const;



	void PlaceSettlement(Coordinate location, Player& Owner);
	void UpgradeSettlement(Coordinate location);
	//void PlaceRoad(Coordinate start, Coordinate end, Player& Owner);


	bool buyRoad(Coordinate start, Coordinate end, Player& Owner);


	//void PlaceSettlement(Coordinate location, Player& Owner);
	void PlaceCity(Coordinate location, Player& Owner);
	bool PlaceRoad(Coordinate start, Coordinate end, Player& Owner);
	
	void accept(GameVisitor& visitor);
	
	bool operator==(const GameBoard& other) const;
    
    bool testRollChecking(int* rolls);

    void moveRobber(Coordinate newRobber);
    Coordinate getRobber() const;

};

#endif
