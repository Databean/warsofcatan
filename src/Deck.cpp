/*
 * Deck.cpp
 *
 *  Created on: Feb 23, 2014
 *      Author: ankitbhutani
 */

#include <iostream>
#include "Deck.h"

/**
 * Construct a Deck with the standard cards available in the Settlers of Catan game.
 */
Deck::Deck()
{
	// TODO Auto-generated constructor stub
	for(int i = 0; i < 15; i++)
	{
		DevelopmentCard* card = new KnightCard();
		this->deck.push_back(card);
	}
	for(int i = 0; i < 4; i++)
	{
		DevelopmentCard* card = new VictoryPointCard();
		this->deck.push_back(card);
	}
	for(int i = 0; i < 2; i++)
	{
		DevelopmentCard* card = new YearOfPlentyCard();
		this->deck.push_back(card);
	}
	for(int i = 0; i < 2; i++)
	{
		DevelopmentCard* card = new MonopolyCard();
		this->deck.push_back(card);
	}
	for(int i = 0; i < 2; i++)
	{
		DevelopmentCard* card = new RoadBuildingCard();
		this->deck.push_back(card);
	}
    
    shuffleDeck();
}

/**
 * Destroy the deck and its held cards.
 */
Deck::~Deck() {
	// TODO Auto-generated destructor stub

	while(!this->deck.empty())
	{
		delete this->deck.back();
		this->deck.pop_back();
		//std::cout<<":";
	}
    
    while(!this->discardPile.empty())
	{
		delete this->discardPile.back();
		this->discardPile.pop_back();
		//std::cout<<":";
	}
}

/**
 * The number of cards in the Deck.
 * @return The amount of cards.
 */
int Deck::getSize()
{
	return this->deck.size();
}

/**
 * Pull a random card from the deck. If the deck is empty, reshuffle it.
 * @return An owning raw pointer to a random card from the deck.
 */
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

/**
 * Randomize the order of the cards in the deck.
 */
void Deck::shuffleDeck()
{
    std::srand(std::time(0));
    random_shuffle(deck.begin(), deck.end());
}

/**
 * Move the cards from the discard pile to the draw pile, and randomize their order.
 */
void Deck::reshuffleDeck()
{
    while(!discardPile.empty())
    {
        deck.push_back(discardPile.back());
        discardPile.pop_back();
    }
    shuffleDeck();
}

/**
 * Return a played card to the discard pile.
 * @param toDiscard An owning raw pointer to the card to discard.
 */
void Deck::discard(DevelopmentCard* toDiscard)
{
    discardPile.push_back(toDiscard);
}
