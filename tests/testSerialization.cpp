#include <UnitTest++.h>

#include "Serialization.h"
#include "GameBoard.h"
#include "Player.h"

#include "tinyxml2.h"

#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

using std::vector;
using std::unique_ptr;
using std::stringstream;

TEST(emptyBoardSerialization) {
	vector<unique_ptr<Player>> players;
	players.emplace_back(unique_ptr<Player>(new Player("test")));
	
	GameBoard testBoard(std::move(players));
	
	stringstream stream;
	testBoard.save(stream);
	
	GameBoard copyBoard(stream);
	
	CHECK(testBoard == copyBoard);
}