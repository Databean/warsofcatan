/*
 * Deck.h
 *
 *  Created on: Feb 23, 2014
 *      Author: ankitbhutani
 */

#ifndef DECK_H_
#define DECK_H_

#include <vector>
#include <ctime>
#include <algorithm>
#include "DevelopmentCard.h"


enum DevCardType { KNIGHT, VICTORYPOINT, YEAROFPLENTY, MONOPOLY, ROADBUILDING };


/**
 * A collection of Settlers of Catan cards, initialized with the cards available in the standard game
 * in the original amounts.
 */
class Deck {

private:
	std::vector<DevCardType> deck;
    std::vector<DevCardType> discardPile;
    
    void shuffleDeck();
    void reshuffleDeck();

public:
	Deck();
	virtual ~Deck();

	int getSize();
	DevCardType drawCard();
    void discard(DevCardType toDiscard);
};

#endif /* DECK_H_ */
