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
