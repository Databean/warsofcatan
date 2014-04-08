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
DevelopmentCard::DevelopmentCard()
{

}

DevelopmentCard::~DevelopmentCard() {
	
}



void DevelopmentCard::accept(GameVisitor& visitor) {
    visitor.visit(*this);
}

bool DevelopmentCard::operator==(const DevelopmentCard& other) {
	return getType() == other.getType();
}

KnightCard::KnightCard():DevelopmentCard()
{

}

DevCardType KnightCard::getType() const {
    return KNIGHT;
}

void KnightCard::playCard(Player *player, Coordinate target)
{
    //Call function to play card
	board->moveRobber(target);
}


VictoryPointCard::VictoryPointCard():DevelopmentCard()
{

}

DevCardType VictoryPointCard::getType() const {
    return VICTORYPOINT;
}



YearOfPlentyCard::YearOfPlentyCard():DevelopmentCard()
{

}

DevCardType YearOfPlentyCard::getType() const {
    return YEAROFPLENTY;
}

void YearOfPlentyCard::playCard(Player *player, int rType1, int rType2)
{
    //Call function to change knight positsion
	player->addResource(rType1, 1);
	player->addResource(rType2, 1);

}



MonopolyCard::MonopolyCard():DevelopmentCard()
{

}

DevCardType MonopolyCard::getType() const {
    return MONOPOLY;
}

void MonopolyCard::playCard(Player *player, int rType)
{
	int totalResourceCount = 0;

	for(int i=0; i<board->getNoOfPlayers(); i++)
	{
		Player& p = board->getPlayer(i);
		totalResourceCount += p.getResource(rType);
		p.addResource(rType, (-1*p.getResource(rType)) );
	}
	player->addResource(rType, totalResourceCount);
}



RoadBuildingCard::RoadBuildingCard():DevelopmentCard(){};

DevCardType RoadBuildingCard::getType() const
{
    return ROADBUILDING;
}



void RoadBuildingCard::playCard(Player* player, Coordinate start1, Coordinate end1, Coordinate start2, Coordinate end2){
	if (!board->PlaceRoad(start1, end1, *player)){
		throw std::invalid_argument("The first road passed was not valid, no roads placed");
	}
	if ((!board->PlaceRoad(start2, end2, *player))){
		throw std::invalid_argument("The second road passed was not valid, only the first road was placed");
	}
}






