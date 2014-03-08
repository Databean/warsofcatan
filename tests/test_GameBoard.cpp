/*
 * test_GameBoard.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: The Pickle
 */

#include <vector>
#include <memory>

#include "GameBoard.h"
#include "Util.h"
#include "UnitTest++.h"

TEST(randomize_rolls_fail)
{
    GameBoard test_board(std::vector<std::unique_ptr<Player>>{});
    int badRolls[] = {0, 2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    CHECK(test_board.testRollChecking(badRolls) == false);
}


TEST(randomize_rolls_pass)
{
    GameBoard test_board(std::vector<std::unique_ptr<Player>>{});
    int goodRolls[] = {9, 11, 5, 4, 0, 3, 4, 2, 10, 8, 3, 6, 9, 11, 5, 10, 6, 12, 8};
    CHECK(test_board.testRollChecking(goodRolls) == true);
}

TEST(place_road_good){
	Coordinate start(0,0);
	Coordinate end(0,1);
	
	std::vector<std::unique_ptr<Player>> players {};
	players.emplace_back(new Player("tester"));
	Player& test_player = *players[0];
	
	GameBoard * test_board = new GameBoard(std::move(players));

	test_board->PlaceRoad(start, end, test_player);
	std::shared_ptr<Road> test_road = test_board->getRoad(start, end);
	if (test_road == NULL)
		CHECK(false);
	else
		CHECK(test_road->equals(start, end));

	delete (test_board);
}

TEST(place_road_badroad){
	std::vector<std::unique_ptr<Player>> players {};
	players.emplace_back(new Player("tester"));
	Player& test_player = *players[0];
	
	GameBoard * test_board = new GameBoard(std::move(players));

	Coordinate start(0,0);
	Coordinate end(0,2);

	test_board->PlaceRoad(start, end, test_player);
	std::shared_ptr<Road> test_road = test_board->getRoad(start, end);

	CHECK(test_road == NULL);

	delete (test_board);
}

TEST(place_road_outofbounds){
	std::vector<std::unique_ptr<Player>> players {};
	players.emplace_back(new Player("tester"));
	Player& test_player = *players[0];
	
	GameBoard * test_board = new GameBoard(std::move(players));

	Coordinate start(0,0);
	Coordinate end(-1,0);

	test_board->PlaceRoad(start, end, test_player);
	std::shared_ptr<Road> test_road = test_board->getRoad(start, end);

	CHECK(test_road == NULL);

	delete (test_board);
}

TEST(place_road_roadexists){
	std::vector<std::unique_ptr<Player>> players {};
	players.emplace_back(new Player("tester"));
	Player& test_player = *players[0];
	
	GameBoard * test_board = new GameBoard(std::move(players));

	Coordinate start(0,0);
	Coordinate end(0,1);

	test_board->PlaceRoad(start, end, test_player);
	std::shared_ptr<Road> test_road = test_board->getRoad(start, end);
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
	std::vector<std::unique_ptr<Player>> players {};
	players.emplace_back(new Player("tester"));
	Player& test_player = *players[0];
	
	GameBoard * test_board = new GameBoard(std::move(players));

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
	std::vector<std::unique_ptr<Player>> players {};
	players.emplace_back(new Player("tester"));
	Player& test_player = *players[0];
	
	GameBoard * test_board = new GameBoard(std::move(players));

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

	CHECK(longest_road == 15);

	delete (test_board);
}

