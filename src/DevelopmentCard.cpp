//
//  DevelopmentCard.cpp
//  Catan parts
//
//  Created by Ankit Bhutani on 2/22/14.
//  Copyright (c) 2014 Ankit Bhutani. All rights reserved.
//

#include "DevelopmentCard.h"

/**
 * DevelopmentCard base class destructor.
 */
DevelopmentCard::~DevelopmentCard() {
	
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
 * The type of card that this is.
 * @return The card's type.
 */
DevCardType KnightCard::getType() const {
    return KNIGHT;
}

/**
 * The type of card that this is.
 * @return The card's type.
 */
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

/**
 * The type of card that this is.
 * @return The card's type.
 */
DevCardType MonopolyCard::getType() const {
    return MONOPOLY;
}


/**
 * The type of card that this is.
 * @return The card's type.
 */
DevCardType RoadBuildingCard::getType() const
{
    return ROADBUILDING;
}







