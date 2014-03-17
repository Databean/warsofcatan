/*
 * test_GameBoard.cpp
 *
 *  Created on: March 10, 2014
 *      Author: malecki1
 */

#include "Deck.h"
#include "Util.h"
#include "UnitTest++.h"

TEST(deck_draw)
{
    Deck* testDeck= new Deck();
    DevelopmentCard* temp = testDeck->drawCard();
    CHECK(temp!=NULL);
    testDeck->discard(temp);
    temp = NULL;
    delete testDeck;
}

TEST(reshuffle_discard_pile)
{
    Deck* testDeck= new Deck();
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