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
    
    shuffle();
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
		return NULL;
	DevelopmentCard* card = this->deck.back();
	this->deck.pop_back();
	return card;
}

void Deck::shuffle()
{
    std::srand(std::time(0));
    random_shuffle(deck.begin(), deck.end());
}
