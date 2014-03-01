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

//#include "DevelopmentCard.h"


class DevelopmentCard;
class Deck;

//#include <ext/hash_map>
//#include <unordered_map>

class Player {
private:
    std::string name;

    std::vector<DevelopmentCard*> developmentCards;

    int armySize;
    int longestRoad;
    int victoryPoints;

    int resources[5];


public:

	Player(std::string playerName);
	~Player();

    int getVictoryPoints();
    void updateVictoryPoints();

    int getDevCardsInHand();

    void buyCard(DevelopmentCard* card);
    void playCard(DevelopmentCard* card);

    int getWood();
    int getBrick();
    int getOre();
    int getWheat();
    int getWool();

    void setWood(int resource);
    void setBrick(int resource);
    void setOre(int resource);
    void setWheat(int resource);
    void setWool(int resource);

};

#endif
