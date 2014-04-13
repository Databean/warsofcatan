//
//  DevelopmentCard.cpp
//  Catan parts
//
//  Created by Ankit Bhutani on 2/22/14.
//  Copyright (c) 2014 Ankit Bhutani. All rights reserved.
//

#include "DevelopmentCard.h"


DevelopmentCard::~DevelopmentCard() {
	
}


void DevelopmentCard::accept(GameVisitor& visitor) {
    visitor.visit(*this);
}

bool DevelopmentCard::operator==(const DevelopmentCard& other) {
	return getType() == other.getType();
}

DevCardType KnightCard::getType() const {
    return KNIGHT;
}


DevCardType VictoryPointCard::getType() const {
    return VICTORYPOINT;
}

/**
 * The type of card that this is.
 * @return The card's type.
 */
DevCardType YearOfPlentyCard::getType() const {
    return YEAROFPLENTY;
}


DevCardType MonopolyCard::getType() const {
    return MONOPOLY;
}


DevCardType RoadBuildingCard::getType() const
{
    return ROADBUILDING;
}






