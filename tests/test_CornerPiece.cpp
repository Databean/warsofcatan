/*
 * test_cornerpiece.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: Kyle Grage
 */

#include "gtest/gtest.h"

#include "CornerPiece.h"
#include "Settlement.h"
#include "City.h"
#include "Wonder.h"
#include "GameBoard.h"
#include "Player.h"

//TEST CONSTRUCTORS
TEST(CornerPieceTest, Settlement_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Settlement test_cp(board, loc, test_player);
	ASSERT_EQ(loc, test_cp.getLocation());
	ASSERT_EQ(board, test_cp.getBoard());
	ASSERT_EQ(1, test_cp.getVictoryPoints());
}

TEST(CornerPieceTest, City_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	City test_cp(board, loc, test_player);
	ASSERT_EQ(loc, test_cp.getLocation());
	ASSERT_EQ(board, test_cp.getBoard());
	ASSERT_EQ(2, test_cp.getVictoryPoints());
}

TEST(CornerPieceTest, Wonder_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Wonder test_cp(board, loc, test_player);
	ASSERT_EQ(loc, test_cp.getLocation());
	ASSERT_EQ(board, test_cp.getBoard());
	ASSERT_EQ(10, test_cp.getVictoryPoints());
}

TEST(CornerPieceTest, City_upgrade_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Settlement intermediate_cp(board, loc, test_player);
	City test_cp(intermediate_cp);
	ASSERT_EQ(loc, test_cp.getLocation());
	ASSERT_EQ(board, test_cp.getBoard());
	ASSERT_EQ(2, test_cp.getVictoryPoints());
}

TEST(CornerPieceTest, Wonder_upgrade_settlement_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Settlement intermediate_cp(board, loc, test_player);
	Wonder test_cp(intermediate_cp);
	ASSERT_EQ(loc, test_cp.getLocation());
	ASSERT_EQ(board, test_cp.getBoard());
	ASSERT_EQ(10, test_cp.getVictoryPoints());
}

TEST(CornerPieceTest, Wonder_upgrade_city_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	City intermediate_cp(board, loc, test_player);
	Wonder test_cp(intermediate_cp);
	ASSERT_EQ(loc, test_cp.getLocation());
	ASSERT_EQ(board, test_cp.getBoard());
	ASSERT_EQ(10, test_cp.getVictoryPoints());
}

//TEST RESOURCE MODIFIERS
TEST(CornerPieceTest, Settlement_Resource_Mod){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Settlement test_cp(board, loc, test_player);
	ASSERT_EQ(1, test_cp.getResourceModifier());
}

TEST(CornerPieceTest, City_Resource_Mod){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	City test_cp(board, loc, test_player);
	ASSERT_EQ(2, test_cp.getResourceModifier());
}

TEST(CornerPieceTest, Wonder_Resource_Mod){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Wonder test_cp(board, loc, test_player);
	ASSERT_EQ(10, test_cp.getResourceModifier());
}

//TEST VICTORY POINTS
TEST(CornerPieceTest, Settlement_Victory_Pts){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Settlement test_cp(board, loc, test_player);
	ASSERT_EQ(1, test_cp.getVictoryPoints());
}

TEST(CornerPieceTest, City_Victory_Pts){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	City test_cp(board, loc, test_player);
	ASSERT_EQ(2, test_cp.getVictoryPoints());
}

TEST(CornerPieceTest, Wonder_Victory_Pts){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Wonder test_cp(board, loc, test_player);
	ASSERT_EQ(10, test_cp.getVictoryPoints());
}


