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

//#include "DevelopmentCard.h"


class DevelopmentCard;
class Deck;

//#include <ext/hash_map>
//#include <unordered_map>

class Player {
private:
    std::string name;

    std::vector<std::unique_ptr<DevelopmentCard>> developmentCards;

    int armySize;
    int longestRoad;
    int victoryPoints;

    int resources[5];


public:

	Player(std::string playerName);
	Player(tinyxml2::XMLElement*);
	~Player();

    int getVictoryPoints();
    void updateVictoryPoints();

    int getDevCardsInHand();

    void buyCard(std::unique_ptr<DevelopmentCard> card);
    std::string getName() const;

    void playCard(DevelopmentCard* card);

    int getWood() const;
    int getBrick() const;
    int getOre() const;
    int getWheat() const;
    int getWool() const;

    void setWood(int resource);
    void setBrick(int resource);
    void setOre(int resource);
    void setWheat(int resource);
    void setWool(int resource);

	void accept(GameVisitor& visitor);
	bool operator==(const Player& player) const;
};

#endif
