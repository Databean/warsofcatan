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
 * @param board The board the player is playing on.
 * @param playerName The name of the player. Should be unique.
 */
Player::Player(GameBoard& board, std::string playerName) : name(playerName), board(board)
{
	armySize = 0;
	longestRoad = 0;
	victoryPoints = 0;
	for(auto& r : resources) {
		r = 0;
	}
	for(auto& t : tradeModifiers) {
		t = 4;
	}
}

/**
 * Construct a player from a serialized tinyxml2::XMLElement.
 * @param board The board the player is playing on.
 * @param elem The XMLElement to read data from.
 */
Player::Player(GameBoard& board, XMLElement* elem) : board(board)
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
			std::pair<std::string, std::function<std::unique_ptr<DevelopmentCard>(void)>>("knight", [&board]() -> std::unique_ptr<DevelopmentCard> { return std::unique_ptr<DevelopmentCard>(new KnightCard(board)); }),
			std::pair<std::string, std::function<std::unique_ptr<DevelopmentCard>(void)>>("victorypoint", [&board]() -> std::unique_ptr<DevelopmentCard> { return std::unique_ptr<DevelopmentCard>(new VictoryPointCard(board)); }),
			std::pair<std::string, std::function<std::unique_ptr<DevelopmentCard>(void)>>("yearofplenty", [&board]() -> std::unique_ptr<DevelopmentCard> { return std::unique_ptr<DevelopmentCard>(new YearOfPlentyCard(board)); }),
			std::pair<std::string, std::function<std::unique_ptr<DevelopmentCard>(void)>>("monopoly", [&board]() -> std::unique_ptr<DevelopmentCard> { return std::unique_ptr<DevelopmentCard>(new MonopolyCard(board)); }),
			std::pair<std::string, std::function<std::unique_ptr<DevelopmentCard>(void)>>("roadbuilding", [&board]() -> std::unique_ptr<DevelopmentCard> { return std::unique_ptr<DevelopmentCard>(new RoadBuildingCard(board)); }),
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
	return getWood() >= 1 && getBrick() >= 1;
}

/**
 * Subtracts the cost of a road from a player's resources if they have enough
 * @return true if the resources were subtracted, false otherwise
 */
bool Player::buyRoad(){
	if(canBuyRoad()){
		addMultiple(-1,-1,0,0,0);
		return true;
	}
	//insufficient funds
	return false;
}

/**
 * Determine if the player has enough resources to buy a settlement.
 * @return True if the player has enough resources to buy a settlement, false otherwise
 */
bool Player::canBuySettlement(){
	return getWood() >= 1 && getBrick() >= 1 && getWheat() >= 1 && getWool() >= 1;
}

/**
 * Subtracts the cost of a road from a player's resources if they have enough
 * @return true if the resources were subtracted, false otherwise
 */
bool Player::buySettlement(){
	if(canBuySettlement()){
		addMultiple(-1,-1,0,-1,-1);
		return true;
	}
	return false;
}

/**
 * Determine if the player has enough resources to buy a city.
 * @return True if the player has enough resources to buy a city, false otherwise
 */
bool Player::canBuyCity(){
	return getWheat() >= 2 && getOre() >= 3;
}

/**
 * Subtracts the cost of a city from a player's resources if they have enough
 * @return true if the resources were subtracted, false otherwise
 */
bool Player::buyCity(){
	if(canBuyCity()){
		addMultiple(0,0,-3,-2,0);
	}
	return false;
}

/**
 * Determine if the player has enough resources to buy a wonder.
 * @return True if the player has enough resources to buy a wonder, false otherwise
 */
bool Player::canBuyWonder(){
	return getWood() >= 5 && getBrick() >= 5 && getWheat() >= 5 && getWool() >= 5 && getOre() >= 5;
}

/**
 * Subtracts the cost of a wonder from a player's resources if they have enough
 * @return true if the resources were subtracted, false otherwise
 */
bool Player::buyWonder(){
	if(canBuyWonder()){
		addMultiple(-5,-5,-5,-5,-5);
		return true;
	}
	return false;
}

/**
 * Determine if the player has enough resources to buy a card.
 * @return True if the player has enough resources to buy a card, false otherwise
 */
bool Player::canBuyCard(){
	return getWheat() >= 1 && getWool() >= 1 && getOre() >= 1;
}

/**
 * Subtracts the cost of a card from a player's resources if they have enough
 * @return true if the resources were subtracted, false otherwise
 */
bool Player::buyCard(){
	if(canBuyCard()){
		addMultiple(0,0,-1,-1,-1);
		return true;
	}
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
	int retVal = 0;
	for(int i =0; i<getDevCardsInHand(); i++)
	{
		if(developmentCards[i]->getType() == VICTORYPOINT)
			retVal++;
	}
	return retVal;
}

int Player::getArmySize(){
	return armySize;
}

int Player::getLongestRoad(){
	return longestRoad;
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
 * Gets the current modifier for trading wood
 * @return int, the trading value for wood
 */
int Player::getWoodModifier(){
	return tradeModifiers[WOOD_INDEX];
}

/**
 * Sets the trade modifier for Wood to 2:1
 */
void Player::setWoodModifier()
{
	tradeModifiers[WOOD_INDEX] = 2;
}

/**
 * Gets the current modifier for trading brick
 * @return int, the trading value for brick
 */
int Player::getBrickModifier(){
	return tradeModifiers[BRICK_INDEX];
}

/**
 * Sets the trade modifier for Brick to 2:1
 */
void Player::setBrickModifier()
{
	tradeModifiers[BRICK_INDEX] = 2;
}

/**
 * Gets the current modifier for trading ore
 * @return int, the trading value for ore
 */
int Player::getOreModifier(){
	return tradeModifiers[ORE_INDEX];
}

/**
 * Sets the trade modifier for Ore to 2:1
 */
void Player::setOreModifier()
{
	tradeModifiers[ORE_INDEX] = 2;
}

/**
 * Gets the current modifier for trading wheat
 * @return int, the trading value for wheat
 */
int Player::getWheatModifier(){
	return tradeModifiers[WHEAT_INDEX];
}

/**
 * Sets the trade modifier for Wheat to 2:1
 */
void Player::setWheatModifier()
{
	tradeModifiers[WHEAT_INDEX] = 2;
}

/**
 * Gets the current modifier for trading wool
 * @return int, the trading value for wool
 */
int Player::getWoolModifier(){
	return tradeModifiers[WOOL_INDEX];
}

/**
 * Sets the trade modifier for Wool to 2:1
 */
void Player::setWoolModifier()
{
	tradeModifiers[WOOL_INDEX] = 2;
}

/**
 * Sets the trade modifier for all resources to 3:1
 */
void Player::setGeneralModifier()
{
	for(int i =0; i<5; i++)
	{
		if(tradeModifiers[i] == 4)
			tradeModifiers[i] = 3;
	}
}


/**
 *Performs a trade with bank according to the trade modifiers for each resource
 *@param offer An array representing your offer to the bank
 *@param demand An array representing your demand
 */
void Player::tradeWithBank(std::array<int, 5> offer, std::array<int, 5> demand)
{
	for(int i=0; i<5; i++)
	{
		resources[i] -= offer[i]*tradeModifiers[i];
		resources[i] += demand[i];
	}
}

/**
 * Offer the bank a trade
 * @param offer An array representing your offer to the bank
 * @param demand An array representing your demand
 * @return true if bank accepted and trade was successful.
 */
bool Player::offerBankTrade(std::array<int, 5> offer, std::array<int, 5> demand)
{
	if(!checkResources(offer.data()))
		return false;

	std::array<int, 5> offerToBank;

	for(int i=0; i<5; i++)
	{
		if(offer[i]%tradeModifiers[i] != 0)
			return false;
		offerToBank[i] = offer[i]/tradeModifiers[i];
	}

	this->tradeWithBank(offerToBank, demand);
	return true;
}


/**
 * Accept the trade offer from another player.
 * @param p The player offering the trade.
 * @param offer The resources the other player is offering.
 * @param demand The resources the other player wants in return.
 */

bool Player::acceptOffer(Player& p, std::array<int, 5> offer, std::array<int, 5> demand)
{
	if(!checkResources(offer.data()) || !p.checkResources(demand.data())) {
		return false;
	}
	p.addWood(demand[WOOD_INDEX] - offer[WOOD_INDEX]);
	p.addBrick(demand[BRICK_INDEX] - offer[BRICK_INDEX]);
	p.addOre(demand[ORE_INDEX] - offer[ORE_INDEX]);
	p.addWheat(demand[WHEAT_INDEX] - offer[WHEAT_INDEX]);
	p.addWool(demand[WOOL_INDEX] - offer[WOOL_INDEX]);

	this->addWood(offer[WOOD_INDEX] - demand[WOOD_INDEX]);
	this->addBrick(offer[BRICK_INDEX] - demand[BRICK_INDEX]);
	this->addOre(offer[ORE_INDEX] - demand[ORE_INDEX]);
	this->addWheat(offer[WHEAT_INDEX] - demand[WHEAT_INDEX]);
	this->addWool(offer[WOOL_INDEX] - demand[WOOL_INDEX]);

	return true;
}


/**
 * picks any one resource at random for robber to steal
 * @return type of resource to steal
 */
int Player::getRandomResource()
{
	//int total = getWood() + getBrick() + getOre() + getWheat() + getWool();
	int randomNo = 0;

	if(getWood()!=0 && randomNo <= getWood())
		return WOOD_INDEX;
	else
		randomNo -= getWood();

	if(getBrick()!=0 && randomNo <= getBrick())
		return BRICK_INDEX;
	else
		randomNo -= getBrick();

	if(getOre()!=0 && randomNo <= getOre())
		return ORE_INDEX;
	else
		randomNo -= getOre();

	if(getWheat()!=0 && randomNo <= getWheat())
		return WHEAT_INDEX;
	else
		randomNo -= getWheat();

	if(getWool()!=0 && randomNo <= getWool())
		return WOOL_INDEX;
	else
		randomNo -= getWool();

	return -1;

}

/**
 * Get the resources a player has of a given type.
 * @param resourceType The index to get the resource count of.
 * @return The amount of the resource the player has.
 */
int Player::getResource(int resourceType) const {
	if(resourceType < 5) {
		return resources[resourceType];
	} else {
		throw std::runtime_error("Type index is out of bounds.");
	}
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
 * Adds (or subtracts) the amount of wood a player has
 * @param resource, the number to add (negative to subtract)
 */
void Player::addWood(int resource)
{
	if(resources[WOOD_INDEX] < (0-resource))
		resources[WOOD_INDEX] = 0;
	else
		resources[WOOD_INDEX] += resource;
}

/**
 * Adds (or subtracts) the amount of brick a player has
 * @param resource, the number to add (negative to subtract)
 */
void Player::addBrick(int resource)
{
	if(resources[BRICK_INDEX] < (0-resource))
			resources[BRICK_INDEX] = 0;
	else
		resources[BRICK_INDEX] += resource;
}

/**
 * Adds (or subtracts) the amount of ore a player has
 * @param resource, the number to add (negative to subtract)
 */
void Player::addOre(int resource)
{
	if(resources[ORE_INDEX] < (0-resource))
			resources[ORE_INDEX] = 0;
	else
		resources[ORE_INDEX] += resource;
}

/**
 * Adds (or subtracts) the amount of wheat a player has
 * @param resource, the number to add (negative to subtract)
 */
void Player::addWheat(int resource)
{
	if(resources[WHEAT_INDEX] < (0-resource))
		resources[WHEAT_INDEX] = 0;
	else
		resources[WHEAT_INDEX] += resource;
}

/**
 * Adds (or subtracts) the amount of wool a player has
 * @param resource, the number to add (negative to subtract)
 */
void Player::addWool(int resource)
{
	if(resources[WOOL_INDEX] < (0-resource))
		resources[WOOL_INDEX] = 0;
	else
		resources[WOOL_INDEX] += resource;
}

/**
 * Adds (or subtracts) the amount of resources a player has
 * Param order: wood, brick, ore, wheat, wool
 * @param [resource], the number to add (negative to subtract)
 *
 */
void Player::addMultiple(int wood, int brick, int ore, int wheat, int wool){
	addWood(wood);
	addBrick(brick);
	addOre(ore);
	addWheat(wheat);
	addWool(wool);
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
	if(resources[resourceType] < (0-delta))
		resources[resourceType] = 0;
	else
		resources[resourceType] += delta;
	
}

/**
 * Check to see if a player's resources are equal to the input
 * @param [resource]x5 the amount of (wood, brick, ore, wheat, wool) you are checking
 * @return bool if the values match
 */
bool Player::validateResourceAmount(int wood, int brick, int ore, int wheat, int wool){
	return wood==getWood() && brick==getBrick() && ore==getOre() && wheat==getWheat() && wool==getWool();
}

/**
 * Check to see if a player's trade modifiers are equal to the input
 * @param [resource]x5 the modifiers (wood, brick, ore, wheat, wool) you are checking
 * @return bool if the values match
 */
bool Player::validateTradeModifiers(int wood, int brick, int ore, int wheat, int wool){
	return wood==getWoodModifier() && brick==getBrickModifier() && ore==getOreModifier()
			&& wheat==getWheatModifier() && wool==getWoolModifier();

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
