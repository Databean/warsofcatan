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
	for(int i = 0; i < 15; i++)
	{
		this->deck.push_back(KNIGHT);
	}
	for(int i = 0; i < 4; i++)
	{
		this->deck.push_back(VICTORYPOINT);
	}
	for(int i = 0; i < 2; i++)
	{
		this->deck.push_back(YEAROFPLENTY);
	}
	for(int i = 0; i < 2; i++)
	{
		this->deck.push_back(MONOPOLY);
	}
	for(int i = 0; i < 2; i++)
	{
		this->deck.push_back(ROADBUILDING);
	}
    
    shuffleDeck();
}

/**
 * Destroy the deck and its held cards.
 */
Deck::~Deck() {
	// TODO Auto-generated destructor stub


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
DevCardType Deck::drawCard()
{
	if(this->getSize() == 0)
    {
        reshuffleDeck();
    }
    
    if(this->getSize() == 0)
    {
        return NULLTYPE;
    }
    
    DevCardType card = this->deck.back();
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
void Deck::discard(DevCardType toDiscard)
{
    discardPile.push_back(toDiscard);
}
