//
//  Player.h
//  Catan parts
//
//  Created by Ankit Bhutani on 2/22/14.
//  Copyright (c) 2014 Ankit Bhutani. All rights reserved.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <memory>
#include <array>

#include "tinyxml2.h"
#include "Util.h"
#include "GameVisitor.h"

// WHEAT, SHEEP, STONE, BRICK, WOOD is the order because it matches the enum in GamePiece.h
const int WHEAT_INDEX = 0;
const int WOOL_INDEX = 1;
const int ORE_INDEX = 2;
const int BRICK_INDEX = 3;
const int WOOD_INDEX = 4;


class DevelopmentCard;
class Deck;


class Player {
private:
    std::string name;

    //std::vector<std::unique_ptr<DevelopmentCard>> developmentCards;
    int developmentCards[5];

    int armySize;
    bool largestArmy;
    int longestRoadSize;
    bool longestRoad;

    int baseVictoryPoints;
    int victoryPoints;
    GameBoard& board;
    int resources[5];
    int tradeModifiers[5];

    std::tuple<float, float, float> color;


    void tradeWithBank(std::array<int, 5> offer, std::array<int, 5> demand);

public:

	Player(GameBoard& board, std::string playerName);
	Player(GameBoard& board, tinyxml2::XMLElement*);
	~Player();

    int getVictoryPoints();

    void updateVictoryPoints();

    int getArmySize() const;
    bool hasLargestArmy() const;
    void setLargestArmy(bool);
    int getLongestRoadSize() const;
    bool hasLongestRoad() const;
    void setLongestRoad(bool);
    void setLongestRoadSize(int);

    std::tuple<float, float, float> getColor() const;

    int getVictoryPointsWithoutCards();
    int getVictoryPointCards();

    int getDevCardsInHand();

    bool buyCard(std::unique_ptr<DevelopmentCard> &card);
    std::string getName() const;
    GameBoard* getBoard();
    void setBoard(GameBoard* newboard);

    //KNIGHT, VICTORYPOINT, YEAROFPLENTY, MONOPOLY, ROADBUILDING
    bool playVictoryCard();
    bool playKnight(Coordinate location, Player& opponent);
    bool playYearOfPlenty(int resource);
    bool playMonopoly(int resource);
    bool playRoadBuilding(Coordinate start1, Coordinate end1, Coordinate start2, Coordinate end2);

    bool canBuyRoad();
    bool buyRoad();
    bool canBuySettlement();
    bool buySettlement();
    bool canBuyCity();
    bool buyCity();
    bool canBuyWonder();
    bool buyWonder();
    bool canBuyCard();
    bool buyCard();

    int getWoodModifier();
    void setWoodModifier();
    int getBrickModifier();
    void setBrickModifier();
    int getOreModifier();
    void setOreModifier();
    int getWheatModifier();
    void setWheatModifier();
    int getWoolModifier();
    void setWoolModifier();

    void setGeneralModifier();

    void setGenralModifier();			//3:1 port

    bool offerBankTrade(std::array<int, 5> offer, std::array<int, 5> demand);

    bool acceptOffer(Player& p, std::array<int, 5> offer, std::array<int, 5> demand);

    int getRandomResource();

    bool checkResources(int resourceList[]);

    int getWood() const;
    int getBrick() const;
    int getOre() const;
    int getWheat() const;
    int getWool() const;

    void setStartingValues();

    int getDevelopmentCards(int card_type) const;
    int getVictoryCards() const;
    int getKnightCards() const;
    int getYearOfPlentyCards() const;
    int getMonopolyCards() const;
    int getRoadBuildingCards() const;



    void addWood(int resource);
    void addBrick(int resource);
    void addOre(int resource);
    void addWheat(int resource);
    void addWool(int resource);
    void addMultiple(int wood, int brick, int ore, int wheat, int wool);

    int getResource(int resourceType) const; //
    void addResource(int resourceType, int delta);
    int giveAllResources(int resourceType);

    bool validateResourceAmount(int wood, int brick, int ore, int wheat, int wool);
    bool validateTradeModifiers(int wood, int brick, int ore, int wheat, int wool);


	void accept(GameVisitor& visitor);
	bool operator==(const Player& player) const;
};

#endif
