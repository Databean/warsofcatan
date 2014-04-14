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

/**
 * A collection of Settlers of Catan cards, initialized with the cards available in the standard game
 * in the original amounts.
 */
class Deck {

private:
	std::vector<DevelopmentCard*> deck;
    std::vector<DevelopmentCard*> discardPile;
    
    void shuffleDeck();
    void reshuffleDeck();

public:
	Deck(GameBoard& board);
	virtual ~Deck();

	int getSize();
	DevelopmentCard* drawCard();
    void discard(DevelopmentCard* toDiscard);
};

#endif /* DECK_H_ */
