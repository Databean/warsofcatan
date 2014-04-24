/*
 * test_GameBoard.cpp
 *
 *  Created on: March 10, 2014
 *      Author: malecki1
 */

#include "gtest/gtest.h"

#include "Deck.h"
#include "Util.h"

TEST(DeckTest, deck_draw)
{
    Deck* testDeck= new Deck();
    DevelopmentCard* temp = testDeck->drawCard();
    ASSERT_NE(temp, nullptr);
    testDeck->discard(temp);
    temp = NULL;
    delete testDeck;
}

TEST(DeckTest, reshuffle_discard_pile)
{
    Deck* testDeck= new Deck();
    DevelopmentCard* drawn = NULL;
    for (int i = 0; i<300; i++)
    {
        drawn = testDeck->drawCard();
        ASSERT_NE(drawn, nullptr);
        testDeck->discard(drawn);
        drawn = NULL;
    }
    delete testDeck;
}