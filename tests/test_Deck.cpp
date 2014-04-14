/*
 * test_GameBoard.cpp
 *
 *  Created on: March 10, 2014
 *      Author: malecki1
 */

#include "Deck.h"
#include "GameBoard.h"
#include "Util.h"
#include "UnitTest++.h"

TEST(deck_draw)
{
	GameBoard board({});
    Deck* testDeck= new Deck(board);
    DevelopmentCard* temp = testDeck->drawCard();
    CHECK(temp!=NULL);
    testDeck->discard(temp);
    temp = NULL;
    delete testDeck;
}

TEST(reshuffle_discard_pile)
{
	GameBoard board({});
    Deck* testDeck= new Deck(board);
    DevelopmentCard* drawn = NULL;
    for (int i = 0; i<300; i++)
    {
        drawn = testDeck->drawCard();
        CHECK(drawn!=NULL);
        testDeck->discard(drawn);
        drawn = NULL;
    }
    delete testDeck;
}