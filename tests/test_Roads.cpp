/*
 * test_Roads.cpp
 *
 *  Created on: Mar 1, 2014
 *      Author: The Pickle
 */

#include "gtest/gtest.h"

#include "Road.h"
#include "Util.h"
#include "GameBoard.h"

TEST(RoadTest, road_constructor_good){
	Coordinate start = Coordinate(0,0);
	Coordinate end = Coordinate(0,1);

	GameBoard board({"tester"});
	Player& test_player = board.getPlayer(0);
	Road test_road(start, end, test_player);

	ASSERT_TRUE(test_road.getStart() == start);
	ASSERT_TRUE(test_road.getEnd() == end);
}

TEST(RoadTest, road_constuctor_bad){
	Coordinate start = Coordinate(0,0);
	Coordinate end = Coordinate(3,4);

	GameBoard board({"tester"});
	Player& test_player = board.getPlayer(0);
	//test start == end
	try {
		Road test_road_1(start, start, test_player);
		ASSERT_TRUE(false);
	} catch (const std::invalid_argument& e) {
		ASSERT_TRUE(true);
	}
	//test road too long
	try {
		Road test_road_2(start, end, test_player);
		ASSERT_TRUE(false);
	} catch (const std::invalid_argument& e) {
		ASSERT_TRUE(true);
	}

}

TEST(RoadTest, road_equals_Road){
	Coordinate start_1 = Coordinate(0,0);
	Coordinate end_1 = Coordinate(0,1);

	GameBoard board({"tester", "tester_2"});
	Player& test_player_1 = board.getPlayer(0);
	Road test_road_1(start_1, end_1, test_player_1);


	Coordinate start_2 = Coordinate(0,1);
	Coordinate end_2 = Coordinate(0,0);
	Player& test_player_2 = board.getPlayer(1);
	Road test_road_2(start_2, end_2, test_player_2);

	Coordinate start_3 = Coordinate(1,1);
	Coordinate end_3 = Coordinate(1,2);
	Road test_road_3(start_3, end_3, test_player_1);

	Road test_road_4(end_1, start_1, test_player_1);

	ASSERT_FALSE(test_road_1.equals(test_road_2));
	ASSERT_FALSE(test_road_1.equals(test_road_3));
	ASSERT_TRUE(test_road_1.equals(test_road_4));
	ASSERT_TRUE(test_road_1.equals(test_road_1));
}

TEST(RoadTest, road_equals_operator){
	Coordinate start_1 = Coordinate(0,0);
	Coordinate end_1 = Coordinate(0,1);

	GameBoard board({"tester", "tester_2"});
	Player& test_player_1 = board.getPlayer(0);
	Road test_road_1(start_1, end_1, test_player_1);


	Coordinate start_2 = Coordinate(0,1);
	Coordinate end_2 = Coordinate(0,0);
	Player& test_player_2 = board.getPlayer(1);
	Road test_road_2(start_2, end_2, test_player_2);

	Coordinate start_3 = Coordinate(1,1);
	Coordinate end_3 = Coordinate(1,2);
	Road test_road_3(start_3, end_3, test_player_1);

	Road test_road_4(end_1, start_1, test_player_1);

	ASSERT_FALSE(test_road_1 == test_road_2);
	ASSERT_FALSE(test_road_1 == test_road_3);
	ASSERT_TRUE(test_road_1 == test_road_4);
	ASSERT_TRUE(test_road_1 == test_road_1);

}

TEST(RoadTest, road_equals_Coordinate){
	Coordinate start_1 = Coordinate(0,0);
	Coordinate end_1 = Coordinate(0,1);

	GameBoard board({"tester"});
	Player& test_player_1 = board.getPlayer(0);
	Road test_road_1(start_1, end_1, test_player_1);


	Coordinate start_2 = Coordinate(0,1);
	Coordinate end_2 = Coordinate(0,0);

	Coordinate start_3 = Coordinate(1,1);
	Coordinate end_3 = Coordinate(1,2);

	ASSERT_TRUE(test_road_1.equals(start_2, end_2));
	ASSERT_FALSE(test_road_1.equals(start_3, end_3));
}

