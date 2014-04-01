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

#include "tinyxml2.h"

#include "GameVisitor.h"

const int WOOD_INDEX = 0;
const int BRICK_INDEX = 1;
const int ORE_INDEX = 2;
const int WHEAT_INDEX = 3;
const int WOOL_INDEX = 4;


class DevelopmentCard;
class Deck;
class GameBoard;


/**
 * One of the players interacting with the Settlers of Catan game. Contains her name, victory points,
 * development cards, and resources.
 */
class Player {
private:
    std::string name;

    std::vector<std::unique_ptr<DevelopmentCard>> developmentCards;

    int armySize;
    int longestRoad;
    int victoryPoints;
    GameBoard* board;
    int resources[5];




public:

	Player(std::string playerName);
	Player(tinyxml2::XMLElement*);
	~Player();

    int getVictoryPoints();
    void updateVictoryPoints();

    int getVictoryPointsWithoutCards();
    int getVictoryPointCards();

    int getDevCardsInHand();

    void buyCard(std::unique_ptr<DevelopmentCard> card);
    std::string getName() const;

    GameBoard* getBoard();
    void setBoard(GameBoard* newboard);

    void playCard(DevelopmentCard* card);

    bool canBuyRoad();
    bool buyRoad();

    bool offerTrade(Player* p, int offer[], int demand[]);
    bool recieveOffer(Player* p, int offer[], int demand[]);
    bool acceptOffer(Player* p, int offer[], int demand[]);

    bool checkResources(int resourceList[]);

    int getWood() const;
    int getBrick() const;
    int getOre() const;
    int getWheat() const;
    int getWool() const;

    void addWood(int resource);
    void addBrick(int resource);
    void addOre(int resource);
    void addWheat(int resource);
    void addWool(int resource);

    void addResource(int resourceType, int delta);

	void accept(GameVisitor& visitor);
	bool operator==(const Player& player) const;
};

#endif
