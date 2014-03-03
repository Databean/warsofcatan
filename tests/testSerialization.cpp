#include <UnitTest++.h>

#include "Serialization.h"
#include "GameBoard.h"
#include "Player.h"

#include "tinyxml2.h"

#include <iostream>
#include <sstream>

using std::stringstream;

TEST(emptyBoardSerialization) {
	GameBoard testBoard;
	
	stringstream stream;
	testBoard.save(stream);
	
	GameBoard copyBoard(stream);
	
	CHECK(testBoard == copyBoard);
}