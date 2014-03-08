//
//  Player.cpp
//  Catan parts
//
//  Created by Ankit Bhutani on 2/22/14.
//  Copyright (c) 2014 Ankit Bhutani. All rights reserved.
//


#include "Player.h"

#include <algorithm>
#include <map>
#include <utility>
#include <stdexcept>
#include <functional>
#include <iostream>

#include "DevelopmentCard.h"

using tinyxml2::XMLElement;
using std::map;
using std::make_pair;
using std::runtime_error;

Player::Player(std::string playerName) : name(playerName)
{
	armySize = 0;
	longestRoad = 0;
	victoryPoints = 0;
	for(auto& r : resources) {
		r = 0;
	}
}

Player::Player(XMLElement* elem)
{
	for(auto& r : resources) {
		r = 0;
	}
	name = elem->FirstChildElement("name")->FirstChild()->Value();
	setWood(fromString<int>(elem->FirstChildElement("wood")->FirstChild()->Value()));
	setBrick(fromString<int>(elem->FirstChildElement("brick")->FirstChild()->Value()));
	setOre(fromString<int>(elem->FirstChildElement("ore")->FirstChild()->Value()));
	setWheat(fromString<int>(elem->FirstChildElement("wheat")->FirstChild()->Value()));
	setWool(fromString<int>(elem->FirstChildElement("wool")->FirstChild()->Value()));
	XMLElement* cardsElement = elem->FirstChildElement("cards");
	for(auto cardElem = cardsElement->FirstChildElement("card"); cardElem; cardElem = cardElem->NextSiblingElement("card")) {
		static const map<std::string, std::function<std::unique_ptr<DevelopmentCard>(void)>> typeToCard = {
			std::pair<std::string, std::function<std::unique_ptr<DevelopmentCard>(void)>>("knight", [this]() -> std::unique_ptr<DevelopmentCard> { return std::unique_ptr<DevelopmentCard>(new KnightCard(this)); }),
			std::pair<std::string, std::function<std::unique_ptr<DevelopmentCard>(void)>>("victorypoint", [this]() -> std::unique_ptr<DevelopmentCard> { return std::unique_ptr<DevelopmentCard>(new VictoryPointCard(this)); }),
			std::pair<std::string, std::function<std::unique_ptr<DevelopmentCard>(void)>>("yearofplenty", [this]() -> std::unique_ptr<DevelopmentCard> { return std::unique_ptr<DevelopmentCard>(new YearOfPlentyCard(this)); }),
			std::pair<std::string, std::function<std::unique_ptr<DevelopmentCard>(void)>>("monopoly", [this]() -> std::unique_ptr<DevelopmentCard> { return std::unique_ptr<DevelopmentCard>(new MonopolyCard(this)); }),
			std::pair<std::string, std::function<std::unique_ptr<DevelopmentCard>(void)>>("roadbuilding", [this]() -> std::unique_ptr<DevelopmentCard> { return std::unique_ptr<DevelopmentCard>(new RoadBuildingCard(this)); }),
		};
		auto typeIt = typeToCard.find(std::string(cardElem->FirstChildElement("type")->FirstChild()->Value()));
		if(typeIt == typeToCard.end()) {
			throw runtime_error("Invalid card type");
		}
		developmentCards.emplace_back(typeIt->second());
	}
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


void Player::buyCard(std::unique_ptr<DevelopmentCard> card)
{
    developmentCards.push_back(std::move(card));
}

void Player::playCard(DevelopmentCard *card)
{
    auto cardTester = [card](std::unique_ptr<DevelopmentCard>& test) -> bool { return card == test.get(); };
    if(!std::any_of(developmentCards.begin(), developmentCards.end(), cardTester)) {
        return;
    }
    card->playCard();
    if (card->getType() == KNIGHT) {
        armySize++;
	}

    std::remove_if(developmentCards.begin(), developmentCards.end(), cardTester);
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


int Player::getWood() const
{
    return resources[WOOD_INDEX];
}

int Player::getBrick() const
{
    return resources[BRICK_INDEX];
}

int Player::getOre() const
{
    return resources[ORE_INDEX];
}

int Player::getWheat() const
{
    return resources[WHEAT_INDEX];
}

int Player::getWool() const
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

std::string Player::getName() const
{
    return name;
}

void Player::accept(GameVisitor& visitor) {
	visitor.visit(*this);
	for(auto& card : developmentCards) {
		card->accept(visitor);
	}
}

bool Player::operator==(const Player& player) const {
	if(developmentCards.size() != player.developmentCards.size()) {
		return false;
	}
	for(std::size_t i = 0; i < developmentCards.size(); i++) {
		if((*developmentCards[i]) == (*player.developmentCards[i])) {}
		else {
			return false;
		}
	}
	return getName() == player.getName() &&
		getWood() == player.getWood() &&
		getBrick() == player.getBrick() &&
		getOre() == player.getOre() &&
		getWheat() == player.getWheat() &&
		getWool() == player.getWool();
}
