//
//  Player.cpp
//  Catan parts
//
//  Created by Ankit Bhutani on 2/22/14.
//  Copyright (c) 2014 Ankit Bhutani. All rights reserved.
//


#include "Player.h"

#include <algorithm>

#include "DevelopmentCard.h"


Player::Player(std::string playerName)
{
	armySize = 0;
	longestRoad = 0;
	victoryPoints = 0;
}

Player::~Player() {

}

int Player::getDevCardsInHand()
{
	return developmentCards.size();
}


void Player::updateVictoryPoints()
{
    //TODO: Calculate and Update victory points
}

int Player::getVictoryPointsWithoutCards()
{
    updateVictoryPoints();
    return victoryPoints - getVictoryPointCards();
}

int Player::getVictoryPointCards()
{
	//TODO:write function
	return 0;
}

int Player::getVictoryPoints()
{
    updateVictoryPoints();
    return victoryPoints;
}


void Player::buyCard(DevelopmentCard *card)
{
    developmentCards.push_back(card);
}

void Player::playCard(DevelopmentCard *card)
{
    if(std::find(developmentCards.begin(), developmentCards.end(), card) == developmentCards.end()) {
        return;
    }
    card->playCard();
    if (card->getType() == KNIGHT)
        armySize++;

    developmentCards.erase(std::remove(developmentCards.begin(), developmentCards.end(), card), developmentCards.end());

}




bool Player::offerTrade(Player* p, int offer[], int demand[])
{
	if(sizeof offer/sizeof(int) != 5 || sizeof demand/sizeof(int) != 5)
		return false; //Invalid Trade

	if(!this->checkResources(offer))
		return false; //YOu dont have enough to offer this

	return p->recieveOffer(this, offer, demand);
}

bool Player::recieveOffer(Player* p, int offer[], int demand[])
{
	if( !this->checkResources(demand) )
		return false;

	bool input = true;

	//TODO:Display Offer to User and wait for input

	if(input)
	{
		this->acceptOffer(p, offer, demand);
		return true;
	}
	else
		return false;

}


bool Player::acceptOffer(Player* p, int offer[], int demand[])
{
	p->setWood(demand[WOOD_INDEX] - offer[WOOD_INDEX]);
	p->setBrick(demand[BRICK_INDEX] - offer[BRICK_INDEX]);
	p->setOre(demand[ORE_INDEX] - offer[ORE_INDEX]);
	p->setWheat(demand[WHEAT_INDEX] - offer[WHEAT_INDEX]);
	p->setWool(demand[WOOL_INDEX] - offer[WOOL_INDEX]);

	this->setWood(offer[WOOD_INDEX] - demand[WOOD_INDEX]);
	this->setBrick(offer[BRICK_INDEX] - demand[BRICK_INDEX]);
	this->setOre(offer[ORE_INDEX] - demand[ORE_INDEX]);
	this->setWheat(offer[WHEAT_INDEX] - demand[WHEAT_INDEX]);
	this->setWool(offer[WOOL_INDEX] - demand[WOOL_INDEX]);

	return true;
}





bool Player::checkResources(int resourceList[5])
{
	for(int i = 0; i < 5; i++)
	{
		if(resourceList[i] > resources[i] || resourceList[i] < 0)
			return false;
	}
	return true;
}


int Player::getWood()
{
    return resources[WOOD_INDEX];
}

int Player::getBrick()
{
    return resources[BRICK_INDEX];
}

int Player::getOre()
{
    return resources[ORE_INDEX];
}

int Player::getWheat()
{
    return resources[WHEAT_INDEX];
}

int Player::getWool()
{
    return resources[WOOL_INDEX];
}



void Player::setWood(int resource)
{
	if(resources[WOOD_INDEX] < (0-resource))
		resources[WOOD_INDEX] = 0;
	else
		resources[WOOD_INDEX] += resource;
}

void Player::setBrick(int resource)
{
	if(resources[BRICK_INDEX] < (0-resource))
			resources[BRICK_INDEX] = 0;
	else
		resources[BRICK_INDEX] += resource;
}

void Player::setOre(int resource)
{
	if(resources[ORE_INDEX] < (0-resource))
			resources[ORE_INDEX] = 0;
	else
		resources[ORE_INDEX] += resource;
}

void Player::setWheat(int resource)
{
	if(resources[WHEAT_INDEX] < (0-resource))
		resources[WHEAT_INDEX] = 0;
	else
		resources[WHEAT_INDEX] += resource;
}

void Player::setWool(int resource)
{
	if(resources[WOOL_INDEX] < (0-resource))
		resources[WOOL_INDEX] = 0;
	else
		resources[WOOL_INDEX] += resource;
}
