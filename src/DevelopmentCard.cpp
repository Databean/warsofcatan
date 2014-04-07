//
//  DevelopmentCard.cpp
//  Catan parts
//
//  Created by Ankit Bhutani on 2/22/14.
//  Copyright (c) 2014 Ankit Bhutani. All rights reserved.
//

#include "DevelopmentCard.h"
#include "Util.h"
#include "GameBoard.h"

/**
 * DevelopmentCard base class constructor.
 * @param player The owner of the card. May be @code nullptr @endcode for a card in the discard pile.
 */
DevelopmentCard::DevelopmentCard(Player* player):owner(player)
{

}

/**
 * DevelopmentCard base class destructor.
 */
DevelopmentCard::~DevelopmentCard() {
	
}

/**
 * Getter for the owner of a development card.
 * @return The card's owner.
 */
Player* DevelopmentCard::getOwner() {
    return owner;
}

/**
 * Visitor double-dispatch method.
 * @param visitor The visiting instance.
 */
void DevelopmentCard::accept(GameVisitor& visitor) {
    visitor.visit(*this);
}

/**
 * Tests equality between two development cards.
 * @param other Card to test equality with.
 * @return If the two cards are equal.
 */
bool DevelopmentCard::operator==(const DevelopmentCard& other) {
	return getType() == other.getType();
}

/**
 * Construct a Knight Card with its owning player.
 * @param player The owner of the card. May be @code nullptr @endcode for a card in the discard pile.
 */
KnightCard::KnightCard(Player* player):DevelopmentCard(player)
{

}

/**
 * The type of card that this is.
 * @return The card's type.
 */
DevCardType KnightCard::getType() const {
    return KNIGHT;
}

/**
 * Move the robber and steal a resource from another player.
 */
void KnightCard::playCard()
{
    //TODO: implement
}

/**
 * 
 */
VictoryPointCard::VictoryPointCard(Player* player):DevelopmentCard(player)
{

}

/**
 * The type of card that this is.
 * @return The card's type.
 */
DevCardType VictoryPointCard::getType() const {
    return VICTORYPOINT;
}

/**
 * Permanently give the owning player a victory point.
 */
void VictoryPointCard::playCard()
{
    //TODO:implement
}


YearOfPlentyCard::YearOfPlentyCard(Player* player):DevelopmentCard(player)
{

}

/**
 * The type of card that this is.
 * @return The card's type.
 */
DevCardType YearOfPlentyCard::getType() const {
    return YEAROFPLENTY;
}

/**
 * Give the owning player two resources of her choice.
 */
void YearOfPlentyCard::playCard()
{
    //TODO: implement
}


/**
 * Construct the monopoly card with its owning player.
 */
MonopolyCard::MonopolyCard(Player* player):DevelopmentCard(player)
{

}

/**
 * The type of card that this is.
 * @return The card's type.
 */
DevCardType MonopolyCard::getType() const {
    return MONOPOLY;
}

/**
 * Collect all the resources of a specific type from other players.
 */
void MonopolyCard::playCard()
{
    //TODO: implement
}


/**
 * Construct a road building card with its owning player.
 * @param player The owning player.
 */
RoadBuildingCard::RoadBuildingCard(Player* player):DevelopmentCard(player){
	
};

/**
 * The type of card that this is.
 * @return The card's type.
 */
DevCardType RoadBuildingCard::getType() const
{
    return ROADBUILDING;
}

/**
 * Build two roads without cost at locations of the owner's choice.
 */
void RoadBuildingCard::playCard()
{
    //Call function to build a road twice
	//Perhaps this function could signal to the controller how it should receive input next.
}

/**
 * Build two roads without cost at the locations of the owner's choice.
 * @throws std::invalid_argument If either of the roads is invalid.
 * @param start1 The starting location of the first road.
 * @param end1 The ending location of the first road.
 * @param start2 The starting location of the second road.
 * @param end2 The ending location of the second road.
 */
void RoadBuildingCard::playCard(Coordinate start1, Coordinate end1, Coordinate start2, Coordinate end2){
	if (!(getOwner()->getBoard()->PlaceRoad(start1, end1, *getOwner()))){
		throw std::invalid_argument("The first road passed was not valid, no roads placed");
	}
	if ((!getOwner()->getBoard()->PlaceRoad(start2, end2, *getOwner()))){
		throw std::invalid_argument("The second road passed was not valid, only the first road was placed");
	}
}






