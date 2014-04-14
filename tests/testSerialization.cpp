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
	GameBoard testBoard({});

	stringstream stream;
	testBoard.save(stream);

	GameBoard copyBoard(stream);

	CHECK(testBoard == copyBoard);
}

TEST(multiplePlayerSerialization) {
	GameBoard testBoard({"test", "test2"});

	stringstream stream;
	testBoard.save(stream);

	GameBoard copyBoard(stream);

	CHECK(testBoard == copyBoard);
}
/*
TEST(testCardSerialization) {
	GameBoard testBoard({"test"});
	Player& testPlayer = testBoard.getPlayer(0);
	testPlayer.buyCard(unique_ptr<DevelopmentCard>(new KnightCard(testBoard)));
	testPlayer.buyCard(unique_ptr<DevelopmentCard>(new VictoryPointCard(testBoard)));
	testPlayer.buyCard(unique_ptr<DevelopmentCard>(new YearOfPlentyCard(testBoard)));
	testPlayer.buyCard(unique_ptr<DevelopmentCard>(new MonopolyCard(testBoard)));
	testPlayer.buyCard(unique_ptr<DevelopmentCard>(new RoadBuildingCard(testBoard)));

	stringstream stream;
	testBoard.save(stream);

	GameBoard copyBoard(stream);

	CHECK(testBoard == copyBoard);
}*/

TEST(roadSerialization) {
	GameBoard testBoard({"test", "test2"});
	Player& firstPlayer = testBoard.getPlayer(0);
	Player& secondPlayer = testBoard.getPlayer(1);

	testBoard.PlaceSettlement(Coordinate(0,0), firstPlayer);
	testBoard.PlaceSettlement(Coordinate(-1,1), secondPlayer);
	testBoard.PlaceRoad(Coordinate(0,0), Coordinate(-1,1), firstPlayer);
	testBoard.PlaceRoad(Coordinate(-1,1), Coordinate(-1,2), secondPlayer);

	stringstream stream;
	testBoard.save(stream);

	GameBoard copyBoard(stream);

	CHECK(testBoard == copyBoard);
}

TEST(settlementSerialization) {
	GameBoard testBoard({"test"});
	Player& player = testBoard.getPlayer(0);

	testBoard.PlaceSettlement(Coordinate(0, 0), player);
	testBoard.PlaceSettlement(Coordinate(1, 1), player);

	stringstream stream;
	testBoard.save(stream);

	GameBoard copyBoard(stream);

	CHECK(testBoard == copyBoard);
}
