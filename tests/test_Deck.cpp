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
    DevCardType temp = testDeck->drawCard();
    ASSERT_NE(temp, NULLTYPE);
    testDeck->discard(temp);
//    temp = NULL;
//    delete testDeck;
}

TEST(DeckTest, reshuffle_discard_pile)
{
    Deck* testDeck= new Deck();
    DevCardType drawn;
    for (int i = 0; i<300; i++)
    {
        drawn = testDeck->drawCard();
        ASSERT_NE(drawn, NULLTYPE);
        testDeck->discard(drawn);
//        drawn = NULL;
    }
    delete testDeck;
}
