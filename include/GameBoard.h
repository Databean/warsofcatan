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
#include <stdexcept>

#include "Util.h"
#include "GamePiece.h"
#include "Settlement.h"
#include "tinyxml2.h"
#include "Road.h"
#include "GameDice.h"
#include "Deck.h"

#include "DevelopmentCard.h"


class GameVisitor;

/**
 * A game of Settlers of Catan, including resource tiles, settlements, cities, roads, and players.
 */
class GameBoard {
private:
	std::map<Coordinate, std::unique_ptr<CornerPiece>> corners;

	std::map<Coordinate, std::unique_ptr<ResourceTile>> resources;

	GameDice dice;

	Deck deck;

	std::map<Coordinate, std::vector<std::shared_ptr<Road>>> roads;

	std::vector<std::unique_ptr<Player>> players;
	Coordinate robber;

	int currentTurn;

	int maxVictoryPoints;

    void addResource(int x, int y, resourceType res, int val);
    bool checkRolls(int* rolls);

	bool isValidBoard() const;


	bool outOfBounds(const Coordinate& coord) const;
	bool roadExists(Coordinate start, Coordinate end) const;
	bool isRoadConnectionPoint(Coordinate point, Player& Owner) const;

	int constructBoardFromFile(std::ifstream &file);
	int constructFileFromBoard(std::ofstream &file);

	void removeRoadEnd(std::shared_ptr<Road> startRoad);
	int FindLongestRoad_FromPoint(Coordinate curr, const Player & owner, std::map<Coordinate, bool>& marked, std::map<Road*, bool>& markedRoads, int length) const;

	void createRing(Coordinate topRight, int sideLength, std::vector<resourceType>& resources, std::vector<int>& rolls);
	void insertTile(Coordinate location, std::vector<resourceType>& resources, std::vector<int>& rolls);
    
    std::pair<int, int> startTurn();
    void enableRobber();
    void payoutResources(int roll);
    
public:
	GameBoard(const std::vector<std::string>& playerNames);
	GameBoard(const std::vector<std::string>& playerNames, const std::map<Coordinate, std::pair<resourceType, int>>& resourceLocations);
	GameBoard(std::istream& in);
	GameBoard(GameBoard&) = delete;
	~GameBoard();
	GameBoard& operator=(GameBoard&) = delete;

	void initializeGame();

	GameDice getDice();

	void save(std::ostream& out);

	void buyCard(Player& owner);
	void discardCard(DevelopmentCard * card);

	ResourceTile& getResourceTile(Coordinate location) const;

	const std::map<Coordinate, std::unique_ptr<ResourceTile>>& getResources() const;

	void endTurn();
	Player& getCurrentPlayer() const;

	int getMaxVictoryPoints();
	void setMaxVictoryPoints(int maxVicPts);
	const std::shared_ptr<Road> getRoad(Coordinate start, Coordinate end) const;
	const std::vector<std::shared_ptr<Road>>& getRoads(Coordinate loc) const;

	int FindLongestRoad(const Player & owner) const;
	void updateLongestRoadPlayer();
	void updateLargestArmyPlayer();

	std::vector<Settlement*> GetNeighboringSettlements(Coordinate location) const;
	std::vector<CornerPiece*> GetNeighboringCorners(Coordinate location) const;

	int CountCornerPoints(Player& owner);

	void PlaceSettlement(Coordinate location, Player& Owner);
	void UpgradeSettlement(Coordinate location);
	void UpgradeToWonder(Coordinate location);
	
	bool verifyRoadPlacement(Coordinate start, Coordinate end, Player& Owner) const;
	bool buyRoad(Coordinate start, Coordinate end, Player& Owner);
	
	bool canPlaceSettlement(const Coordinate& location, const Player& owner);
	bool buySettlement(const Coordinate& location, Player& owner);
	
	bool canUpgradeSettlement(Coordinate location, const Player& owner) const;
	bool buyUpgradeOnSettlement(Coordinate location, Player& owner);
	
	bool canUpgradeToWonder(Coordinate location, const Player& owner) const;
	bool buyUpgradeOnWonder(Coordinate location, Player& owner);

	//void PlaceSettlement(Coordinate location, Player& Owner);
	void PlaceCity(Coordinate location, Player& Owner);
	void PlaceWonder(Coordinate location, Player& Owner);
	bool PlaceRoad(Coordinate start, Coordinate end, Player& Owner);
	bool canPlayBuildRoadCard(Coordinate start1, Coordinate end1, Coordinate start2, Coordinate end2, Player& Owner);

	void accept(GameVisitor& visitor);

	bool operator==(const GameBoard& other) const;

	const std::vector<std::unique_ptr<Player>>& getPlayers() const;
	
	int getNoOfPlayers();
	Player& getPlayer(int index);

    bool testRollChecking(int* rolls);

    bool moveRobber(Coordinate newRobber);
    Coordinate getRobber() const;
    bool canRobberRob(Player& opponent, Coordinate location);

};

#endif
