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




KnightCard::KnightCard(Player* player):DevelopmentCard(player)
{

}

DevCardType KnightCard::getType(){
    return KNIGHT;
}

void KnightCard::playCard()
{
    //Call function to play card
}


VictoryPointCard::VictoryPointCard(Player* player):DevelopmentCard(player)
{

}

DevCardType VictoryPointCard::getType(){
    return VICTORYPOINT;
}

void VictoryPointCard::playCard()
{
    //Call function to change knight positsion
}


YearOfPlentyCard::YearOfPlentyCard(Player* player):DevelopmentCard(player)
{

}

DevCardType YearOfPlentyCard::getType(){
    return YEAROFPLENTY;
}

void YearOfPlentyCard::playCard()
{
    //Call function to change knight positsion
}



MonopolyCard::MonopolyCard(Player* player):DevelopmentCard(player)
{

}

DevCardType MonopolyCard::getType(){
    return MONOPOLY;
}

void MonopolyCard::playCard()
{
    //Call function to change knight positsion
}



RoadBuildingCard::RoadBuildingCard(Player* player):DevelopmentCard(player){};

DevCardType RoadBuildingCard::getType()
{
    return ROADBUILDING;
}

void RoadBuildingCard::playCard()
{
    //Call function to build a road twice
}







