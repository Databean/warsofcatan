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

class Deck {

private:
	std::vector<DevelopmentCard*> deck;
    
    void shuffle();

public:
	Deck();
	virtual ~Deck();

	int getSize();
	DevelopmentCard* drawCard();
};

#endif /* DECK_H_ */
