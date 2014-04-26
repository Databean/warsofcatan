/*
 * test_GameBoard.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: The Pickle
 */

#include <vector>
#include <memory>
#include <map>
#include <stdexcept>

#include "gtest/gtest.h"

#include "GameBoard.h"
#include "GamePiece.h"
#include "Util.h"

TEST(GameBoardTest, randomize_rolls_fail)
{
	
	int xcoords[] = {-2, 0, 2, -3, -1, 1, 3, -4, -2, 0, 2, 4, -3, -1,  1,  3, -2, 0, 2};
    int ycoords[] = { 2, 1, 0,  4,  3, 2, 1,  6,  5, 4, 3, 2,  7,  6,  5,  4,  8, 7, 6};
    int badRolls[] = {0, 2, 3, 3, 4, 4, 5, 5, 6,  6, 8, 8, 9,  9, 10, 10, 11, 11, 12};
	resourceType resources[] = {DESERT, BRICK, BRICK, BRICK, STONE, STONE, STONE, WHEAT, WHEAT, WHEAT, WHEAT, WOOD, WOOD, WOOD, WOOD, SHEEP, SHEEP, SHEEP, SHEEP};
	std::map<Coordinate, std::pair<resourceType, int>> resourcesMap {};
	for(int i = 0; i < 19; i++) {
		resourcesMap[Coordinate{xcoords[i], ycoords[i]}] = std::pair<resourceType, int>(resources[i], badRolls[i]);
	}
	try {
		GameBoard test_board({}, resourcesMap);
		ASSERT_TRUE(false);
	} catch(const std::runtime_error& error) {
		ASSERT_TRUE(true);
	}
}


TEST(GameBoardTest, randomize_rolls_pass)
{
	int xcoords[] = {-2, 0, 2, -3, -1, 1, 3, -4, -2, 0, 2, 4, -3, -1, 1, 3, -2, 0, 2};
    int ycoords[] = { 2, 1, 0,  4,  3, 2, 1,  6, 5,  4, 3, 2,  7,  6, 5, 4,  8, 7, 6};
    int goodRolls[] = {9, 11, 5, 4, 0, 3, 4, 2, 10, 8, 3, 6, 9, 11, 5, 10, 6, 12, 8};
	resourceType resources[] = {BRICK, BRICK, BRICK, STONE, DESERT, STONE, STONE, WHEAT, WHEAT, WHEAT, WHEAT, WOOD, WOOD, WOOD, WOOD, SHEEP, SHEEP, SHEEP, SHEEP};
	std::map<Coordinate, std::pair<resourceType, int>> resourcesMap {};
	for(int i = 0; i < 19; i++) {
		resourcesMap[Coordinate{xcoords[i], ycoords[i]}] = std::pair<resourceType, int>(resources[i], goodRolls[i]);
	}
    GameBoard test_board({}, resourcesMap);
	ASSERT_TRUE(true); //constructor should not have thrown an exception
}

TEST(GameBoardTest, place_road_good){
	Coordinate start(0,0);
	Coordinate end(1,0);
	
	GameBoard test_board({"tester"});
	Player& test_player = test_board.getPlayer(0);

	test_board.PlaceSettlement(start, test_player);
	test_board.PlaceRoad(start, end, test_player);
	std::shared_ptr<Road> test_road = test_board.getRoad(start, end);
	if (test_road == nullptr)
		ASSERT_TRUE(false);
	else
		ASSERT_TRUE(test_road->equals(start, end));
}

TEST(GameBoardTest, place_road_badroad){
	GameBoard test_board(std::vector<std::string>{"tester"});
	Player& test_player = test_board.getPlayer(0);


	Coordinate start(0,0);
	Coordinate end(0,2);

	test_board.PlaceSettlement(start, test_player);
	test_board.PlaceRoad(start, end, test_player);
	std::shared_ptr<Road> test_road = test_board.getRoad(start, end);

	ASSERT_EQ(test_road, nullptr);
}

TEST(GameBoardTest, place_road_outofbounds){
	GameBoard test_board(std::vector<std::string>{"tester"});
	Player& test_player = test_board.getPlayer(0);


	Coordinate start(0,0);
	Coordinate end(-1,0);

	test_board.PlaceSettlement(start, test_player);
	test_board.PlaceRoad(start, end, test_player);
	std::shared_ptr<Road> test_road = test_board.getRoad(start, end);

	ASSERT_EQ(test_road, nullptr);
}

TEST(GameBoardTest, place_road_roadexists){
	GameBoard test_board(std::vector<std::string>{"tester"});
	Player& test_player = test_board.getPlayer(0);


	Coordinate start(0,0);
	Coordinate end(1,0);

	test_board.PlaceSettlement(start, test_player);
	test_board.PlaceRoad(start, end, test_player);
	std::shared_ptr<Road> test_road = test_board.getRoad(start, end);
	if (test_road == nullptr)
		ASSERT_TRUE(false);
	else
		ASSERT_TRUE(test_road->equals(start, end));

	test_board.PlaceRoad(start, end, test_player);
	test_road = test_board.getRoad(start, end);
	if (test_road == nullptr)
		ASSERT_TRUE(false);
	else
		ASSERT_TRUE(test_road->equals(start, end));

}

TEST(GameBoardTest, place_road_noConnectionPoint){
	Coordinate start(0,0);
	Coordinate end(1,0);


	GameBoard test_board(std::vector<std::string>{"tester"});
	Player& test_player = test_board.getPlayer(0);

	test_board.PlaceRoad(start, end, test_player);
	std::shared_ptr<Road> test_road = test_board.getRoad(start, end);
	ASSERT_EQ(test_road, nullptr);
}

TEST(GameBoardTest, longest_road_simple){
	GameBoard test_board(std::vector<std::string>{"tester"});
	Player& test_player = test_board.getPlayer(0);



	int longest_path = 0;
	longest_path = test_board.FindLongestRoad(test_player);
	ASSERT_EQ(longest_path, 0);

	Coordinate start(0,0);
	Coordinate end(-1,1);
	test_board.PlaceSettlement(start, test_player);
	test_board.PlaceRoad(start, end, test_player);
	longest_path = test_board.FindLongestRoad(test_player);
	ASSERT_EQ(longest_path, 1);


	start = Coordinate(-1,1);
	end = Coordinate(-1,2);
	test_board.PlaceRoad(start, end, test_player);
	longest_path = test_board.FindLongestRoad(test_player);
	ASSERT_EQ(longest_path, 2);
}

TEST(GameBoardTest, payout_simple) {
	GameBoard test_board(std::vector<std::string>{"tester"});
	Player& test_player = test_board.getPlayer(0);
	
	test_board.PlaceSettlement(Coordinate(0,2), test_player);
	
	test_board.getResourceTile(Coordinate(0,1)).Payout();
	
	ASSERT_TRUE(test_player.getWheat() || test_player.getWood() || 
		test_player.getOre() || test_player.getBrick() || test_player.getWool());
}

TEST(GameBoardTest, upgrade_simple){
	GameBoard test_board(std::vector<std::string>{"tester"});
	Player& test_player = test_board.getPlayer(0);



	test_board.PlaceSettlement(Coordinate(0,2), test_player);

	test_board.PlaceSettlement(Coordinate(1,0), test_player);

	test_board.UpgradeSettlement(Coordinate(1,0));

	ASSERT_EQ((test_board.GetNeighboringSettlements(Coordinate(0,1)).size()), 1);
}

TEST(GameBoardTest, longest_road_complex){
	GameBoard test_board(std::vector<std::string>{"tester"});
	Player& test_player = test_board.getPlayer(0);


	//(0,0), (-1,1), (-1, 2), (0, 2) (0, 3) (1, 3) (2, 2) (2, 1) (1, 1) (1, 0) (0, 0)
	//						  (-2,3) (-2,4) (-1,5) (0, 5)        (0, 2)

	test_board.PlaceSettlement(Coordinate(0,0), test_player);
	test_board.PlaceRoad(Coordinate(0,0), Coordinate(-1,1), test_player);
	test_board.PlaceRoad(Coordinate(-1,1), Coordinate(-1,2), test_player);
	test_board.PlaceRoad(Coordinate(-1,2), Coordinate(0,2), test_player);
	test_board.PlaceRoad(Coordinate(0,2), Coordinate(0,3), test_player);
	test_board.PlaceRoad(Coordinate(0,3), Coordinate(1,3), test_player);
	test_board.PlaceRoad(Coordinate(1,3), Coordinate(2,2), test_player);
	test_board.PlaceRoad(Coordinate(2,2), Coordinate(2,1), test_player);
	test_board.PlaceRoad(Coordinate(2,1), Coordinate(1,1), test_player);
	test_board.PlaceRoad(Coordinate(1,1), Coordinate(1,0), test_player);
	test_board.PlaceRoad(Coordinate(1,0), Coordinate(0,0), test_player);
	test_board.PlaceRoad(Coordinate(0,2), Coordinate(-2,3), test_player);
	test_board.PlaceRoad(Coordinate(-2,3), Coordinate(-2,4), test_player);
	test_board.PlaceRoad(Coordinate(-2,4), Coordinate(-1,4), test_player);
	test_board.PlaceRoad(Coordinate(-1,4), Coordinate(-1,5), test_player);
	test_board.PlaceRoad(Coordinate(-1,5), Coordinate(0,5), test_player);
	test_board.PlaceRoad(Coordinate(1,1), Coordinate(0,2), test_player);

	int longest_road = test_board.FindLongestRoad(test_player);

	ASSERT_EQ(longest_road, 15);
}

TEST(GameBoardTest, buy_road_good){
	Coordinate start(0,0);
	Coordinate end(1,0);

	GameBoard test_board({"tester"});
	Player& test_player = test_board.getPlayer(0);


	test_board.PlaceSettlement(start, test_player);

	test_player.addBrick(1);
	test_player.addWood(1);
	test_board.buyRoad(start, end, test_player);

	std::shared_ptr<Road> test_road = test_board.getRoad(start, end);
	if (test_road == nullptr)
		ASSERT_TRUE(false);
	else{
		ASSERT_TRUE(test_road->equals(start, end));
		ASSERT_EQ(test_player.getWood(), 0);
		ASSERT_EQ(test_player.getBrick(), 0);
	}
}

TEST(GameBoardTest, canPlayRoadBuildCard){
	Coordinate start1(0,0);
	Coordinate end1(1,0);

	Coordinate start2(1,0);
	Coordinate end2(1,1);

	GameBoard test_board({"tester"});
	Player& test_player = test_board.getPlayer(0);

	test_board.PlaceSettlement(start1, test_player);

	ASSERT_TRUE(test_board.canPlayBuildRoadCard(start1, end1, start2, end2, test_player));
	ASSERT_TRUE(test_board.canPlayBuildRoadCard(end1, start1, start2, end2, test_player));
	ASSERT_TRUE(test_board.canPlayBuildRoadCard(start1, end1, end2, start2, test_player));
	ASSERT_TRUE(test_board.canPlayBuildRoadCard(end1, start1, end2, start2, test_player));
	ASSERT_TRUE(test_board.canPlayBuildRoadCard(start2, end2, start1, end1, test_player));

	ASSERT_FALSE(test_board.canPlayBuildRoadCard(start1, end2, start2, end1, test_player));
	ASSERT_FALSE(test_board.canPlayBuildRoadCard(start1, start1, start2, start2, test_player));
}


TEST(GameBoardTest, countCornerPoint){
	Coordinate pt1(0,0);
	Coordinate pt2(1,0);
	Coordinate pt3(0,2);
	Coordinate pt4(0,3);

	GameBoard test_board({"tester1", "tester2"});
	Player& test_player1 = test_board.getPlayer(0);
	Player& test_player2 = test_board.getPlayer(1);

	ASSERT_EQ(test_board.CountCornerPoints(test_player1), 0);
	ASSERT_EQ(test_board.CountCornerPoints(test_player2), 0);

	test_board.PlaceSettlement(pt1, test_player1);
	test_board.PlaceSettlement(pt2, test_player1);
	test_board.PlaceSettlement(pt3, test_player1);
	test_board.PlaceSettlement(pt4, test_player2);
	test_board.UpgradeSettlement(pt4);

	ASSERT_EQ(test_board.CountCornerPoints(test_player1), 3);
	ASSERT_EQ(test_board.CountCornerPoints(test_player2), 2);
}


TEST(GameBoardTest, updateLongestRoadPlayer){

	GameBoard test_board({"tester1", "tester2"});
	Player& test_player1 = test_board.getPlayer(0);
	Player& test_player2 = test_board.getPlayer(1);

	ASSERT_FALSE(test_player1.hasLongestRoad());
	ASSERT_FALSE(test_player2.hasLongestRoad());

	Coordinate start1(0,0);
	Coordinate start2(0,6);

	test_board.PlaceSettlement(start1, test_player1);
	test_board.PlaceSettlement(start2, test_player2);

	test_board.PlaceRoad(start1, Coordinate(-1,1), test_player1);
	test_board.PlaceRoad(Coordinate(-1,1), Coordinate(-1,2), test_player1);
	test_board.PlaceRoad(Coordinate(-1,2), Coordinate(0,2), test_player1);
	test_board.PlaceRoad(Coordinate(0,2), Coordinate(0,3), test_player1);
	test_board.PlaceRoad(Coordinate(0,3), Coordinate(1,3), test_player1);

	test_board.updateLongestRoadPlayer();
	ASSERT_TRUE(test_player1.hasLongestRoad());
	ASSERT_FALSE(test_player2.hasLongestRoad());

	test_board.PlaceRoad(start2, Coordinate(1,6), test_player2);
	test_board.PlaceRoad(Coordinate(1,6), Coordinate(2,5), test_player2);
	test_board.PlaceRoad(Coordinate(2,5), Coordinate(2,4), test_player2);
	test_board.PlaceRoad(Coordinate(2,4), Coordinate(1,4), test_player2);
	test_board.PlaceRoad(Coordinate(1,4), Coordinate(0,5), test_player2);

	test_board.updateLongestRoadPlayer();
	ASSERT_TRUE(test_player1.hasLongestRoad());
	ASSERT_FALSE(test_player2.hasLongestRoad());
}

/**
TEST(canRobberRob){
	GameBoard test_board({"tester1", "tester2"});
	Player& test_player1 = test_board.getPlayer(0);
	Player& test_player2 = test_board.getPlayer(1);


	ASSERT_FALSE(test_board.canRobberRob(test_player1, Coordinate(0,1)));

	test_board.PlaceSettlement(Coordinate(0,0), test_player1);
	ASSERT_TRUE(test_board.canRobberRob(test_player1, Coordinate(0,1)));
}
**/

TEST(GameBoardTest, buyCard){
	GameBoard test_board({"tester1"});
	Player& test_player = test_board.getPlayer(0);

	ASSERT_TRUE(test_player.getDevCardsInHand() == 0);
	test_board.buyCard(test_player);
	ASSERT_TRUE(test_player.getDevCardsInHand() == 0);

	test_player.addOre(1);
	test_player.addWheat(1);
	test_player.addWool(1);

	test_board.buyCard(test_player);
	ASSERT_TRUE(test_player.getDevCardsInHand() == 1);
}










