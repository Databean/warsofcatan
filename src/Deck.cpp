/*
 * Deck.cpp
 *
 *  Created on: Feb 23, 2014
 *      Author: ankitbhutani
 */

#include <iostream>
#include "Deck.h"

Deck::Deck()
{
	// TODO Auto-generated constructor stub
	for(int i = 0; i < 15; i++)
	{
		DevelopmentCard* card = new KnightCard(NULL);
		this->deck.push_back(card);
	}
	for(int i = 0; i < 4; i++)
	{
		DevelopmentCard* card = new VictoryPointCard(NULL);
		this->deck.push_back(card);
	}
	for(int i = 0; i < 2; i++)
	{
		DevelopmentCard* card = new YearOfPlentyCard(NULL);
		this->deck.push_back(card);
	}
	for(int i = 0; i < 2; i++)
	{
		DevelopmentCard* card = new MonopolyCard(NULL);
		this->deck.push_back(card);
	}
	for(int i = 0; i < 2; i++)
	{
		DevelopmentCard* card = new RoadBuildingCard(NULL);
		this->deck.push_back(card);
	}
    
    shuffleDeck();
}

Deck::~Deck() {
	// TODO Auto-generated destructor stub

	while(!this->deck.empty())
	{
		delete this->deck.back();
		this->deck.pop_back();
		std::cout<<":";
	}
}


int Deck::getSize()
{
	return this->deck.size();
}


DevelopmentCard* Deck::drawCard()
{
	if(this->getSize() == 0)
    {
        reshuffleDeck();
    }
    
    if(this->getSize() == 0)
    {
        return NULL;
    }
    
	DevelopmentCard* card = this->deck.back();
	this->deck.pop_back();
	return card;
}

void Deck::shuffleDeck()
{
    std::srand(std::time(0));
    random_shuffle(deck.begin(), deck.end());
}

void Deck::reshuffleDeck()
{
    while(!discardPile.empty())
    {
        deck.push_back(discardPile.back());
        discardPile.pop_back();
    }
    shuffleDeck();
}

void Deck::discard(DevelopmentCard* toDiscard)
{
    discardPile.push_back(toDiscard);
}
