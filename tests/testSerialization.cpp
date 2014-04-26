#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

#include "gtest/gtest.h"

#include "Serialization.h"
#include "GameBoard.h"
#include "DevelopmentCard.h"
#include "Player.h"

#include "tinyxml2.h"

using std::vector;
using std::unique_ptr;
using std::stringstream;

TEST(SerializationTest, emptyBoardSerialization) {
	GameBoard testBoard({});

	stringstream stream;
	testBoard.save(stream);

	GameBoard copyBoard(stream);
	ASSERT_EQ(testBoard, copyBoard);
}

TEST(SerializationTest, multiplePlayerSerialization) {
	GameBoard testBoard({"test", "test2"});
	stringstream stream;
	testBoard.save(stream);

	GameBoard copyBoard(stream);
	
	ASSERT_EQ(testBoard, copyBoard);
}

TEST(SerializationTest, testCardSerialization) {
	GameBoard testBoard({"test"});
	Player& testPlayer = testBoard.getPlayer(0);


	std::unique_ptr<DevelopmentCard> knight_card = unique_ptr<DevelopmentCard>(new KnightCard());
	std::unique_ptr<DevelopmentCard> victory_card = unique_ptr<DevelopmentCard>(new VictoryPointCard());
	std::unique_ptr<DevelopmentCard> plenty_card = unique_ptr<DevelopmentCard>(new YearOfPlentyCard());
	std::unique_ptr<DevelopmentCard> monopoly_card = unique_ptr<DevelopmentCard>(new MonopolyCard());
	std::unique_ptr<DevelopmentCard> road_card = std::unique_ptr<DevelopmentCard>(new RoadBuildingCard());

	testPlayer.buyCard(knight_card);
	testPlayer.buyCard(victory_card);
	testPlayer.buyCard(plenty_card);
	testPlayer.buyCard(monopoly_card);
	testPlayer.buyCard(road_card);

	stringstream stream;
	testBoard.save(stream);

	GameBoard copyBoard(stream);
	ASSERT_EQ(testBoard, copyBoard);
}

TEST(SerializationTest, roadSerialization) {
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
	ASSERT_EQ(testBoard, copyBoard);
}

TEST(SerializationTest, settlementSerialization) {
	GameBoard testBoard({"test"});
	Player& player = testBoard.getPlayer(0);

	testBoard.PlaceSettlement(Coordinate(0, 0), player);
	testBoard.PlaceSettlement(Coordinate(1, 1), player);

	stringstream stream;
	testBoard.save(stream);

	GameBoard copyBoard(stream);
	ASSERT_EQ(testBoard, copyBoard);
}
