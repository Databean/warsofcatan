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


//Base Class
DevelopmentCard::DevelopmentCard(Player* player):owner(player)
{

}

DevelopmentCard::~DevelopmentCard() {
	
}

Player* DevelopmentCard::getOwner() {
    return owner;
}

void DevelopmentCard::accept(GameVisitor& visitor) {
    visitor.visit(*this);
}

bool DevelopmentCard::operator==(const DevelopmentCard& other) {
	return getType() == other.getType();
}

KnightCard::KnightCard(Player* player):DevelopmentCard(player)
{

}

DevCardType KnightCard::getType() const {
    return KNIGHT;
}

void KnightCard::playCard()
{
    //Call function to play card
}


VictoryPointCard::VictoryPointCard(Player* player):DevelopmentCard(player)
{

}

DevCardType VictoryPointCard::getType() const {
    return VICTORYPOINT;
}

void VictoryPointCard::playCard()
{
    //Call function to change knight positsion
}


YearOfPlentyCard::YearOfPlentyCard(Player* player):DevelopmentCard(player)
{

}

DevCardType YearOfPlentyCard::getType() const {
    return YEAROFPLENTY;
}

void YearOfPlentyCard::playCard()
{
    //Call function to change knight positsion
}



MonopolyCard::MonopolyCard(Player* player):DevelopmentCard(player)
{

}

DevCardType MonopolyCard::getType() const {
    return MONOPOLY;
}

void MonopolyCard::playCard()
{
    //Call function to change knight positsion
}



RoadBuildingCard::RoadBuildingCard(Player* player):DevelopmentCard(player){};

DevCardType RoadBuildingCard::getType() const
{
    return ROADBUILDING;
}

void RoadBuildingCard::playCard()
{
    //Call function to build a road twice
	//Perhaps this function could signal to the controller how it should receive input next.
}

void RoadBuildingCard::playCard(Coordinate start1, Coordinate end1, Coordinate start2, Coordinate end2){
	if (!getOwner()->getBoard()->PlaceRoad(start1, end1, *getOwner())){
		throw std::invalid_argument("The first road passed was not valid, no roads placed");
	}
	if (!getOwner()->getBoard()->PlaceRoad(start2, end2, *getOwner())){
		throw std::invalid_argument("The second road passed was not valid, only the first road was placed");
	}
}






