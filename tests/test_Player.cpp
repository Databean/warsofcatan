/*
 * test_PlayerPurchase.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: Kyle Grage
 */

#include "UnitTest++.h"
#include "Player.h"

const int WOOD_INDEX = 0;
const int BRICK_INDEX = 1;
const int ORE_INDEX = 2;
const int WHEAT_INDEX = 3;
const int WOOL_INDEX = 4;


TEST(Player_constructor){
	Player tp(NULL, "Buster");
	CHECK_EQUAL("Buster", tp.getName());
	CHECK_EQUAL(0, tp.getLongestRoad());
	CHECK_EQUAL(0, tp.getArmySize());
	CHECK_EQUAL(0, tp.getDevCardsInHand());

	CHECK(tp.validateResourceAmount(0,0,0,0,0));
	CHECK(tp.validateTradeModifiers(4,4,4,4,4));
}

TEST(Adders_Positive){
	Player tp(NULL, "Buster");
	tp.addBrick(1);
	tp.addWood(1);
	tp.addOre(1);
	tp.addWheat(1);
	tp.addWool(1);
	CHECK(tp.validateResourceAmount(1,1,1,1,1));
	tp.addMultiple(1,1,1,1,1);
	CHECK(tp.validateResourceAmount(2,2,2,2,2));
}

TEST(Adders_Negative_Normal){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	tp.addBrick(-1);
	tp.addWood(-1);
	tp.addOre(-1);
	tp.addWheat(-1);
	tp.addWool(-1);
	CHECK(tp.validateResourceAmount(4,4,4,4,4));
	tp.addMultiple(-1,-1,-1,-1,-1);
	CHECK(tp.validateResourceAmount(3,3,3,3,3));
}

TEST(Adders_Negative_Excessive){
	Player tp(NULL, "Buster");
	tp.addMultiple(1,1,1,1,1);
	tp.addBrick(-2);
	tp.addWood(-2);
	tp.addOre(-2);
	tp.addWheat(-2);
	tp.addWool(-2);
	CHECK(tp.validateResourceAmount(0,0,0,0,0));
	tp.addMultiple(1,1,1,1,1);
	tp.addMultiple(-2,-2,-2,-2,-2);
	CHECK(tp.validateResourceAmount(0,0,0,0,0));
}

//TRADE MODIFIERS
TEST(Trade_Modifiers_Brick){
	Player tp(NULL, "Buster");
	tp.setBrickModifier();
	CHECK(tp.validateTradeModifiers(4,2,4,4,4));
}

TEST(Trade_Modifiers_Wood){
	Player tp(NULL, "Buster");
	tp.setWoodModifier();
	CHECK(tp.validateTradeModifiers(2,4,4,4,4));
}

TEST(Trade_Modifiers_Ore){
	Player tp(NULL, "Buster");
	tp.setOreModifier();
	CHECK(tp.validateTradeModifiers(4,4,2,4,4));
}

TEST(Trade_Modifiers_Wheat){
	Player tp(NULL, "Buster");
	tp.setWheatModifier();
	CHECK(tp.validateTradeModifiers(4,4,4,2,4));
}

TEST(Trade_Modifiers_Wool){
	Player tp(NULL, "Buster");
	tp.setWoolModifier();
	CHECK(tp.validateTradeModifiers(4,4,4,4,2));
}

TEST(Trade_Modifiers_3){
	Player tp(NULL, "Buster");
	tp.setGeneralModifier();
	CHECK(tp.validateTradeModifiers(3,3,3,3,3));
}

TEST(Trade_Modifiers_Mixed){
	Player tp(NULL, "Buster");
	tp.setWheatModifier();
	tp.setOreModifier();
	tp.setGeneralModifier();
	CHECK(tp.validateTradeModifiers(3,3,2,2,3));
}

//PLAYER PURCHASES
TEST(Buy_Settlement_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	CHECK_EQUAL(true, tp.canBuySettlement());
	tp.buySettlement();
	CHECK(tp.validateResourceAmount(4,4,5,4,4));
}

TEST(Buy_Settlement_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(0,1,1,1,1);
	CHECK_EQUAL(false, tp.canBuySettlement());
	tp.buySettlement();
	CHECK(tp.validateResourceAmount(0,1,1,1,1));
}

TEST(Buy_Road_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	CHECK_EQUAL(true, tp.canBuyRoad());
	tp.buyRoad();
	CHECK(tp.validateResourceAmount(4,4,5,5,5));
}

TEST(Buy_Road_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(0,1,1,1,1);
	CHECK_EQUAL(false, tp.canBuyRoad());
	tp.buyRoad();
	CHECK(tp.validateResourceAmount(0,1,1,1,1));
}

TEST(Buy_City_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	CHECK_EQUAL(true, tp.canBuyCity());
	tp.buyCity();
	CHECK(tp.validateResourceAmount(5,5,2,3,5));
}

TEST(Buy_City_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(1,1,1,1,1);
	CHECK_EQUAL(false, tp.canBuyCity());
	tp.buyCity();
	CHECK(tp.validateResourceAmount(1,1,1,1,1));
}

TEST(Buy_Wonder_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(6,6,6,6,6);
	CHECK_EQUAL(true, tp.canBuyWonder());
	tp.buyWonder();
	CHECK(tp.validateResourceAmount(1,1,1,1,1));
}

TEST(Buy_Wonder_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,1,5,5);
	CHECK_EQUAL(false, tp.canBuyWonder());
	tp.buyWonder();
	CHECK(tp.validateResourceAmount(5,5,1,5,5));
}

TEST(Buy_DevCard_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	CHECK_EQUAL(true, tp.canBuyCard());
	tp.buyCard();
	CHECK(tp.validateResourceAmount(5,5,4,4,4));
}

TEST(Buy_DevCard_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(1,1,0,1,1);
	CHECK_EQUAL(false, tp.canBuyCard());
	tp.buyCard();
	CHECK(tp.validateResourceAmount(1,1,0,1,1));
}


//INSERT VICTORY POINT TESTS HERE


//INSERT DEV CARD TESTS HERE


//INSERT TRADING TESTS HERE


