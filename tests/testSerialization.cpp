#include <UnitTest++.h>

#include "Serialization.h"
#include "GameBoard.h"
#include "DevelopmentCard.h"
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
	
	GameBoard testBoard(std::move(players));
	
	stringstream stream;
	testBoard.save(stream);
	
	GameBoard copyBoard(stream);
	
	CHECK(testBoard == copyBoard);
}

TEST(multiplePlayerSerialization) {
	vector<unique_ptr<Player>> players;
	players.emplace_back(unique_ptr<Player>(new Player("test")));
	players.emplace_back(unique_ptr<Player>(new Player("test2")));
	
	GameBoard testBoard(std::move(players));
	
	stringstream stream;
	testBoard.save(stream);
	
	GameBoard copyBoard(stream);
	
	CHECK(testBoard == copyBoard);
}

TEST(testCardSerialization) {
	vector<unique_ptr<Player>> players;
	players.emplace_back(unique_ptr<Player>(new Player("test")));
	players[0]->buyCard(unique_ptr<DevelopmentCard>(new KnightCard(players[0].get())));
	players[0]->buyCard(unique_ptr<DevelopmentCard>(new VictoryPointCard(players[0].get())));
	players[0]->buyCard(unique_ptr<DevelopmentCard>(new YearOfPlentyCard(players[0].get())));
	players[0]->buyCard(unique_ptr<DevelopmentCard>(new MonopolyCard(players[0].get())));
	players[0]->buyCard(unique_ptr<DevelopmentCard>(new RoadBuildingCard(players[0].get())));
	
	GameBoard testBoard(std::move(players));
	
	stringstream stream;
	testBoard.save(stream);
	
	GameBoard copyBoard(stream);
	
	CHECK(testBoard == copyBoard);
}
