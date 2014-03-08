/*
 * test_GameBoard.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: The Pickle
 */

#include "GameBoard.h"
#include "Util.h"
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

TEST(place_road_good){
	Coordinate start(0,0);
	Coordinate end(0,1);
	Player test_player("tester");

	GameBoard * test_board = new GameBoard();

	test_board->PlaceRoad(start, end, test_player);
	Road * test_road = test_board->getRoad(start, end);
	if (test_road == NULL)
		CHECK(false);
	else
		CHECK(test_road->equals(start, end));

	delete (test_board);
}

TEST(place_road_badroad){
	Player test_player("tester");

	GameBoard * test_board = new GameBoard();

	Coordinate start(0,0);
	Coordinate end(0,2);

	test_board->PlaceRoad(start, end, test_player);
	Road * test_road = test_board->getRoad(start, end);

	CHECK(test_road == NULL);

	delete (test_board);
}

TEST(place_road_outofbounds){
	Player test_player("tester");

	GameBoard * test_board = new GameBoard();

	Coordinate start(0,0);
	Coordinate end(-1,0);

	test_board->PlaceRoad(start, end, test_player);
	Road * test_road = test_board->getRoad(start, end);

	CHECK(test_road == NULL);

	delete (test_board);
}

TEST(place_road_roadexists){
	Player test_player("tester");

	GameBoard * test_board = new GameBoard();

	Coordinate start(0,0);
	Coordinate end(0,1);

	test_board->PlaceRoad(start, end, test_player);
	Road * test_road = test_board->getRoad(start, end);
	if (test_road == NULL)
		CHECK(false);
	else
		CHECK(test_road->equals(start, end));

	test_board->PlaceRoad(start, end, test_player);
	test_road = test_board->getRoad(start, end);
	if (test_road == NULL)
		CHECK(false);
	else
		CHECK(test_road->equals(start, end));

	delete (test_board);

}

TEST(longest_road_simple){
	GameBoard * test_board = new GameBoard();
	Player test_player("tester");

	int longest_path = 0;
	longest_path = test_board->FindLongestRoad(test_player);
	CHECK(longest_path == 0);

	Coordinate start(0,0);
	Coordinate end(-1,1);
	test_board->PlaceRoad(start, end, test_player);
	longest_path = test_board->FindLongestRoad(test_player);
	CHECK(longest_path == 1);


	start = Coordinate(-1,1);
	end = Coordinate(-1,2);
	test_board->PlaceRoad(start, end, test_player);
	longest_path = test_board->FindLongestRoad(test_player);
	CHECK(longest_path == 2);

	delete (test_board);
}


TEST(longest_road_complex){
	GameBoard * test_board = new GameBoard();
	Player test_player("tester");

	//(0,0), (-1,1), (-1, 2), (0, 2) (0, 3) (1, 3) (2, 2) (2, 1) (1, 1) (1, 0) (0, 0)
	//						  (-2,3) (-2,4) (-1,5) (0, 5)        (0, 2)

	test_board->PlaceRoad(Coordinate(0,0), Coordinate(-1,1), test_player);
	test_board->PlaceRoad(Coordinate(-1,1), Coordinate(-1,2), test_player);
	test_board->PlaceRoad(Coordinate(-1,2), Coordinate(0,2), test_player);
	test_board->PlaceRoad(Coordinate(0,2), Coordinate(0,3), test_player);
	test_board->PlaceRoad(Coordinate(0,3), Coordinate(1,3), test_player);
	test_board->PlaceRoad(Coordinate(1,3), Coordinate(2,2), test_player);
	test_board->PlaceRoad(Coordinate(2,2), Coordinate(2,1), test_player);
	test_board->PlaceRoad(Coordinate(2,1), Coordinate(1,1), test_player);
	test_board->PlaceRoad(Coordinate(1,1), Coordinate(1,0), test_player);
	test_board->PlaceRoad(Coordinate(1,0), Coordinate(0,0), test_player);
	test_board->PlaceRoad(Coordinate(0,2), Coordinate(-2,3), test_player);
	test_board->PlaceRoad(Coordinate(-2,3), Coordinate(-2,4), test_player);
	test_board->PlaceRoad(Coordinate(-2,4), Coordinate(-1,4), test_player);
	test_board->PlaceRoad(Coordinate(-1,4), Coordinate(-1,5), test_player);
	test_board->PlaceRoad(Coordinate(-1,5), Coordinate(0,5), test_player);
	test_board->PlaceRoad(Coordinate(1,1), Coordinate(0,2), test_player);

	int longest_road = test_board->FindLongestRoad(test_player);
	std::cout << longest_road;

	CHECK(longest_road == 15);

	delete (test_board);
}















