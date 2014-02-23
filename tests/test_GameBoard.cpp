/*
 * test_GameBoard.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: The Pickle
 */

#include "GameBoard.h"
#include "UnitTest++.h"


TEST(saveFile_simple){
	GameBoard * test_board = new GameBoard;
	CHECK(test_board->save_Board("test_1") == 0);
	delete(test_board);
}

TEST(loadFile_simple){
	GameBoard * test_board = new GameBoard;
	CHECK(test_board->load_Board("test_1") == 0);
	delete(test_board);
}
