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

/**
 * Initialize a player.
 * @param playerName The name of the player. Should be unique.
 */
Player::Player(std::string playerName) : name(playerName)
{
	armySize = 0;
	longestRoad = 0;
	victoryPoints = 0;
	for(auto& r : resources) {
		r = 0;
	}
}

/**
 * Construct a player from a serialized tinyxml2::XMLElement.
 * @param elem The XMLElement to read data from.
 */
Player::Player(XMLElement* elem)
{
	for(auto& r : resources) {
		r = 0;
	}
	name = elem->FirstChildElement("name")->FirstChild()->Value();
	addWood(fromString<int>(elem->FirstChildElement("wood")->FirstChild()->Value()));
	addBrick(fromString<int>(elem->FirstChildElement("brick")->FirstChild()->Value()));
	addOre(fromString<int>(elem->FirstChildElement("ore")->FirstChild()->Value()));
	addWheat(fromString<int>(elem->FirstChildElement("wheat")->FirstChild()->Value()));
	addWool(fromString<int>(elem->FirstChildElement("wool")->FirstChild()->Value()));
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

/**
 * Destroy the player.
 */
Player::~Player() {

}

/**
 * The number of development cards the player is holding.
 * @return The number of cards.
 */
int Player::getDevCardsInHand()
{
	return developmentCards.size();
}

/**
 * Determine if the player has enough resources to buy a road.
 * @return True if the player has enough resources to buy a road, false otherwise
 */
bool Player::canBuyRoad(){
	return getWood() > 0 && getBrick() > 0;
}

/**
 * Subtracts the cost of a road from a player's resources if they have enough
 * @return True if the resources were subtracted, false otherwise.
 */
bool Player::buyRoad(){
	if(canBuyRoad()){
		addWood(-1);
		addBrick(-1);
		return true;
	}
	//insufficient funds
	return false;
}


/**
 * Update the player's internal state with their victory states.
 */
void Player::updateVictoryPoints()
{
    //TODO: Calculate and Update victory points
}

/**
 * The number of victory points a player has, not counting victory point cards.
 * @return Victory points sans cards.
 */
int Player::getVictoryPointsWithoutCards()
{
    updateVictoryPoints();
    return victoryPoints - getVictoryPointCards();
}

/**
 * The number of victory points the player has from victory point cards.
 * @return Victory points from cards.
 */
int Player::getVictoryPointCards()
{
	//TODO:write function
	return 0;
}

/**
 * The number of victory points a player has.
 */
int Player::getVictoryPoints()
{
    updateVictoryPoints();
    return victoryPoints;
}

/**
 * The GameBoard that a player is playing on.
 * @return The board.
 */
GameBoard* Player::getBoard(){
	return board;
}

/**
 * Assign the Player to a particular GameBoard.
 * @param newboard The new board they are playing on.
 */
void Player::setBoard(GameBoard * newboard){
	board = newboard;
}

/**
 * Acquire a development card.
 * @param card An owning pointer to the card the player acquired.
 */
void Player::buyCard(std::unique_ptr<DevelopmentCard> card)
{
    developmentCards.push_back(std::move(card));
}

/**
 * Play a particular development card.
 * @param card The card to play.
 */
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

/**
 * Offer a trade to another player with an offer and a demand.
 * @param p The other player that is receiving the trade.
 * @param offer The resources this player is offering to the other player.
 * @param demand The resources that this player wants in return from the other player.
 * @return If the trade succeeded.
 */
bool Player::offerTrade(Player* p, int offer[], int demand[])
{
	if(sizeof offer/sizeof(int) != 5 || sizeof demand/sizeof(int) != 5)
		return false; //Invalid Trade

	if(!this->checkResources(offer))
		return false; //YOu dont have enough to offer this

	return p->recieveOffer(this, offer, demand);
}

/**
 * Receive a trade offer from another player.
 * @param p The player offering the trade.
 * @param offer The resources the other player is giving.
 * @param demand The resources the other player wants in return.
 * @return If the trade succeeded.
 */
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

/**
 * Accept the trade offer from another player.
 * @param p The player offering the trade.
 * @param offer The resources the other player is offering.
 * @param demand The resources the other player wants in return.
 */
bool Player::acceptOffer(Player* p, int offer[], int demand[])
{
	p->addWood(demand[WOOD_INDEX] - offer[WOOD_INDEX]);
	p->addBrick(demand[BRICK_INDEX] - offer[BRICK_INDEX]);
	p->addOre(demand[ORE_INDEX] - offer[ORE_INDEX]);
	p->addWheat(demand[WHEAT_INDEX] - offer[WHEAT_INDEX]);
	p->addWool(demand[WOOL_INDEX] - offer[WOOL_INDEX]);

	this->addWood(offer[WOOD_INDEX] - demand[WOOD_INDEX]);
	this->addBrick(offer[BRICK_INDEX] - demand[BRICK_INDEX]);
	this->addOre(offer[ORE_INDEX] - demand[ORE_INDEX]);
	this->addWheat(offer[WHEAT_INDEX] - demand[WHEAT_INDEX]);
	this->addWool(offer[WOOL_INDEX] - demand[WOOL_INDEX]);

	return true;
}




/**
 * Determine if the player has a valid (nonnegative) set of resources.
 * @return If the player's resources are valid.
 */
bool Player::checkResources(int resourceList[5])
{
	for(int i = 0; i < 5; i++)
	{
		if(resourceList[i] > resources[i] || resourceList[i] < 0)
			return false;
	}
	return true;
}

/**
 * The amount of wood a player has.
 * @return The player's wood.
 */
int Player::getWood() const
{
    return resources[WOOD_INDEX];
}

/**
 * The amount of brick a player has.
 * @return The player's brick.
 */
int Player::getBrick() const
{
    return resources[BRICK_INDEX];
}

/**
 * The amount of ore a player has.
 * @return The player's ore.
 */
int Player::getOre() const
{
    return resources[ORE_INDEX];
}

/**
 * The amount of wheat a player has.
 * @return The player's wheat.
 */
int Player::getWheat() const
{
    return resources[WHEAT_INDEX];
}

/**
 * The amount of wool a player has.
 * @return The player's wool.
 */
int Player::getWool() const
{
    return resources[WOOL_INDEX];
}

/**
 * Modify a player's wood supply.
 * @param resource The wood to add or remove.
 */
void Player::addWood(int resource)
{
	if(resources[WOOD_INDEX] < (0-resource))
		resources[WOOD_INDEX] = 0;
	else
		resources[WOOD_INDEX] += resource;
}

/**
 * Modify a player's brick supply.
 * @param resource The brick to add or remove.
 */
void Player::addBrick(int resource)
{
	if(resources[BRICK_INDEX] < (0-resource))
			resources[BRICK_INDEX] = 0;
	else
		resources[BRICK_INDEX] += resource;
}

/**
 * Modify a player's ore supply.
 * @param resource The ore to add or remove.
 */
void Player::addOre(int resource)
{
	if(resources[ORE_INDEX] < (0-resource))
			resources[ORE_INDEX] = 0;
	else
		resources[ORE_INDEX] += resource;
}

/**
 * Modify a player's wheat supply.
 * @param resource The wheat to add or remove.
 */
void Player::addWheat(int resource)
{
	if(resources[WHEAT_INDEX] < (0-resource))
		resources[WHEAT_INDEX] = 0;
	else
		resources[WHEAT_INDEX] += resource;
}

/**
 * Modify a player's wool supply.
 * @param resource The wool to add or remove.
 */
void Player::addWool(int resource)
{
	if(resources[WOOL_INDEX] < (0-resource))
		resources[WOOL_INDEX] = 0;
	else
		resources[WOOL_INDEX] += resource;
}

/**
 * Get a player's name.
 * @return The player's name.
 */
std::string Player::getName() const
{
    return name;
}

/**
 * Modify a player's resource supply.
 * @param resourceType The resource to modify the supply of.
 * @param delta The change in the resource.
 */
void Player::addResource(int resourceType, int delta) {
	resources[resourceType] += delta;
	
}

/**
 * Visitor double-dispatch method.
 * @param visitor The visiting instance.
 */
void Player::accept(GameVisitor& visitor) {
	visitor.visit(*this);
	for(auto& card : developmentCards) {
		card->accept(visitor);
	}
}

/**
 * Compare equality with another player.
 * @param player The player to test equality with.
 * @return If the other player is equivalent to this player.
 */
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
