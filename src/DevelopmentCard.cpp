//
//  DevelopmentCard.cpp
//  Catan parts
//
//  Created by Ankit Bhutani on 2/22/14.
//  Copyright (c) 2014 Ankit Bhutani. All rights reserved.
//

#include "DevelopmentCard.h"
#include "Util.h"


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
}







