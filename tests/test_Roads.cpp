/*
 * test_Roads.cpp
 *
 *  Created on: Mar 1, 2014
 *      Author: The Pickle
 */
#include "UnitTest++.h"
#include "Road.h"
#include "Util.h"
#include "GameBoard.h"

TEST(road_constructor_good){
	Coordinate start = Coordinate(0,0);
	Coordinate end = Coordinate(0,1);
<<<<<<< HEAD

=======
	
>>>>>>> refs/heads/master
	GameBoard board({"tester"});
	Player& test_player = board.getPlayer(0);
	Road test_road(start, end, test_player);

	CHECK(test_road.getStart() == start);
	CHECK(test_road.getEnd() == end);
}

TEST(road_constuctor_bad){
	Coordinate start = Coordinate(0,0);
	Coordinate end = Coordinate(3,4);
<<<<<<< HEAD

=======
	
>>>>>>> refs/heads/master
	GameBoard board({"tester"});
	Player& test_player = board.getPlayer(0);
	//test start == end
	try {
		Road test_road_1(start, start, test_player);
		CHECK(false);
	} catch (const std::invalid_argument& e) {
		CHECK(true);
	}
	//test road too long
	try {
		Road test_road_2(start, end, test_player);
		CHECK(false);
	} catch (const std::invalid_argument& e) {
		CHECK(true);
	}

}

TEST(road_equals_Road){
	Coordinate start_1 = Coordinate(0,0);
	Coordinate end_1 = Coordinate(0,1);
<<<<<<< HEAD

=======
	
>>>>>>> refs/heads/master
	GameBoard board({"tester", "tester_2", "tester_3"});
	Player& test_player_1 = board.getPlayer(0);
	Road test_road_1(start_1, end_1, test_player_1);


	Coordinate start_2 = Coordinate(0,1);
	Coordinate end_2 = Coordinate(0,0);
	Player& test_player_2 = board.getPlayer(1);
	Road test_road_2(start_2, end_2, test_player_2);

	Coordinate start_3 = Coordinate(1,1);
	Coordinate end_3 = Coordinate(1,2);
	Player& test_player_3 = board.getPlayer(2);
	Road test_road_3(start_3, end_3, test_player_3);

	CHECK(test_road_1.equals(test_road_2));
	CHECK(!test_road_1.equals(test_road_3));
}

TEST(road_equals_Coordinate){
	Coordinate start_1 = Coordinate(0,0);
	Coordinate end_1 = Coordinate(0,1);
<<<<<<< HEAD

=======
	
>>>>>>> refs/heads/master
	GameBoard board({"tester"});
	Player& test_player_1 = board.getPlayer(0);
	Road test_road_1(start_1, end_1, test_player_1);


	Coordinate start_2 = Coordinate(0,1);
	Coordinate end_2 = Coordinate(0,0);

	Coordinate start_3 = Coordinate(1,1);
	Coordinate end_3 = Coordinate(1,2);

	CHECK(test_road_1.equals(start_2, end_2));
	CHECK(!test_road_1.equals(start_3, end_3));
}

