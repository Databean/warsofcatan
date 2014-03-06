/*
 * test_GameBoard.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: The Pickle
 */

#include "GameBoard.h"
#include "UnitTest++.h"


TEST(saveFile_simple){
	GameBoard test_board;
	CHECK(test_board.save_Board("test_1") == 0);
}

TEST(loadFile_simple){
	GameBoard test_board;
	CHECK(test_board.load_Board("test_1") == 0);
}

TEST(randomize_rolls_fail)
{
    GameBoard test_board;
    int badRolls[] = {0, 2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    CHECK(test_board.testRollChecking(badRolls) == false);
}

TEST(randomize_rolls_pass)
{
    GameBoard test_board;
    int goodRolls[] = {9, 11, 5, 4, 0, 3, 4, 2, 10, 8, 3, 6, 9, 11, 5, 10, 6, 12, 8};
    CHECK(test_board.testRollChecking(goodRolls) == true);
}