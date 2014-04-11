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
}

TEST(Settlement_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Settlement test_cp(&board, loc, test_player);
}

TEST(City_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	City test_cp(&board, loc, test_player);
}

TEST(Wonder_constructor){
	Coordinate loc = Coordinate(0,0);
	GameBoard board({"test board"});
	Player& test_player = board.getPlayer(0);

	Wonder test_cp(&board, loc, test_player);
}

TEST(City_upgrade_constructor){

}

TEST(Wonder_upgrade_constructor){

}

//TEST RESOURCE MODIFIERS
TEST(CornerPiece_Resource_Mod){

}

TEST(Settlement_Resource_Mod){

}

TEST(City_Resource_Mod){

}

TEST(Wonder_Resource_Mod){

}

//TEST VICTORY POINTS
TEST(CornerPiece_Victory_Pts){

}

TEST(Settlement_Victory_Pts){

}

TEST(City_Victory_Pts){

}

TEST(Wonder_Victory_Pts){

}


