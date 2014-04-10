/*
 * test_GameBoard.cpp
 *
 *  Created on: March 10, 2014
 *      Author: malecki1
 */

#include "gtest/gtest.h"

#include "Deck.h"
#include "GameBoard.h"
#include "Util.h"

TEST(DeckTest, deck_draw)
{
	GameBoard board({});
    Deck* testDeck= new Deck(board);
    DevelopmentCard* temp = testDeck->drawCard();
    ASSERT_NE(temp, nullptr);
    testDeck->discard(temp);
    temp = NULL;
    delete testDeck;
}

TEST(DeckTest, reshuffle_discard_pile)
{
	GameBoard board({});
    Deck* testDeck= new Deck(board);
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