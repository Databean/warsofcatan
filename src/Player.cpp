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


Player::Player(std::string playerName) : name(playerName)
{
	armySize = 0;
	longestRoad = 0;
	victoryPoints = 0;
	for(auto& r : resources) {
		r = 0;
	}
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




int Player::getWood() const
{
    return resources[0];
}

int Player::getBrick() const
{
    return resources[1];
}

int Player::getOre() const
{
    return resources[2];
}

int Player::getWheat() const
{
    return resources[3];
}

int Player::getWool() const
{
    return resources[4];
}



void Player::setWood(int resource)
{
    resources[0] = resource;
}

void Player::setBrick(int resource)
{
    resources[1] = resource;
}

void Player::setOre(int resource)
{
    resources[2] = resource;
}

void Player::setWheat(int resource)
{
    resources[3] = resource;
}

void Player::setWool(int resource)
{
    resources[4] = resource;
}

std::string Player::getName() const
{
    return name;
}

void Player::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

bool Player::operator==(const Player& player) const {
	return getName() == player.getName() &&
		getWood() == player.getWood() &&
		getBrick() == player.getBrick() &&
		getOre() == player.getOre() &&
		getWheat() == player.getWheat() &&
		getWool() == player.getWool();
}
