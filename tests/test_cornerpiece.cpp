/*
 * test_cornerpiece.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: Kyle Grage
 */

#include "UnitTest++.h"
#include "CornerPiece.h"
#include "Settlement.h"
#include "City.h"
#include "Wonder.h"

//TEST CONSTRUCTORS
TEST(CornerPiece_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	CornerPiece test_cp(&board, loc, test_player);
	CHECK_EQUAL(loc, test_cp.getLocation());
	CHECK_EQUAL(&board, test_cp.getBoard());
	CHECK_EQUAL(&test_player, test_cp.getOwner());
	CHECK_EQUAL(0, test_cp.getVictoryPoints());
}

TEST(Settlement_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Settlement test_cp(&board, loc, test_player);
	CHECK_EQUAL(loc, test_cp.getLocation());
	CHECK_EQUAL(&board, test_cp.getBoard());
	CHECK_EQUAL(&test_player, test_cp.getOwner());
	CHECK_EQUAL(1, test_cp.getVictoryPoints());
}

TEST(City_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	City test_cp(&board, loc, test_player);
	CHECK_EQUAL(loc, test_cp.getLocation());
	CHECK_EQUAL(&board, test_cp.getBoard());
	CHECK_EQUAL(&test_player, test_cp.getOwner());
	CHECK_EQUAL(2, test_cp.getVictoryPoints());
}

TEST(Wonder_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Wonder test_cp(&board, loc, test_player);
	CHECK_EQUAL(loc, test_cp.getLocation());
	CHECK_EQUAL(&board, test_cp.getBoard());
	CHECK_EQUAL(&test_player, test_cp.getOwner());
	CHECK_EQUAL(10, test_cp.getVictoryPoints());
}

TEST(City_upgrade_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Settlement intermediate_cp(&board, loc, test_player);
	City test_cp(&intermediate_cp);
	CHECK_EQUAL(loc, test_cp.getLocation());
	CHECK_EQUAL(&board, test_cp.getBoard());
	CHECK_EQUAL(&test_player, test_cp.getOwner());
	CHECK_EQUAL(2, test_cp.getVictoryPoints());
}

TEST(Wonder_upgrade_settlement_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Settlement intermediate_cp(&board, loc, test_player);
	Wonder test_cp(&intermediate_cp);
	CHECK_EQUAL(loc, test_cp.getLocation());
	CHECK_EQUAL(&board, test_cp.getBoard());
	CHECK_EQUAL(&test_player, test_cp.getOwner());
	CHECK_EQUAL(10, test_cp.getVictoryPoints());
}

TEST(Wonder_upgrade_city_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	City intermediate_cp(&board, loc, test_player);
	Wonder test_cp(&intermediate_cp);
	CHECK_EQUAL(loc, test_cp.getLocation());
	CHECK_EQUAL(&board, test_cp.getBoard());
	CHECK_EQUAL(&test_player, test_cp.getOwner());
	CHECK_EQUAL(10, test_cp.getVictoryPoints());
}

//TEST RESOURCE MODIFIERS
TEST(CornerPiece_Resource_Mod){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	CornerPiece test_cp(&board, loc, test_player);
	CHECK_EQUAL(0, test_cp.getResourceModifier());
}

TEST(Settlement_Resource_Mod){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Settlement test_cp(&board, loc, test_player);
	CHECK_EQUAL(1, test_cp.getResourceModifier());
}

TEST(City_Resource_Mod){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	City test_cp(&board, loc, test_player);
	CHECK_EQUAL(2, test_cp.getResourceModifier());
}

TEST(Wonder_Resource_Mod){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Wonder test_cp(&board, loc, test_player);
	CHECK_EQUAL(10, test_cp.getResourceModifier());
}

//TEST VICTORY POINTS
TEST(CornerPiece_Victory_Pts){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	CornerPiece test_cp(&board, loc, test_player);
	CHECK_EQUAL(0, test_cp.getVictoryPoints());
}

TEST(Settlement_Victory_Pts){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Settlement test_cp(&board, loc, test_player);
	CHECK_EQUAL(1, test_cp.getVictoryPoints());
}

TEST(City_Victory_Pts){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	City test_cp(&board, loc, test_player);
	CHECK_EQUAL(2, test_cp.getVictoryPoints());
}

TEST(Wonder_Victory_Pts){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Wonder test_cp(&board, loc, test_player);
	CHECK_EQUAL(10, test_cp.getVictoryPoints());
}


