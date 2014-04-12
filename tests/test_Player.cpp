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

/**
 * Check to see if a player's resources are equal to the input
 * @param [resource]x5 the amount of (wood, brick, ore, wheat, wool) you are checking
 * @param player
 * @return bool if the values match
 */
bool validateResourceAmount(int wood, int brick, int ore, int wheat, int wool, Player tp){
	return wood==tp.getWood() && brick==tp.getBrick() && ore==tp.getOre() && wheat==tp.getWheat() && wool==tp.getWool();
}

/**
 * Check to see if a player's trade modifiers are equal to the input
 * @param [resource]x5 the modifiers (wood, brick, ore, wheat, wool) you are checking
 * @param player
 * @return bool if the values match
 */
bool validateTradeModifiers(int wood, int brick, int ore, int wheat, int wool, Player tp){
	return wood==tp.getWoodModifier() && brick==tp.getBrickModifier() && ore==tp.getOreModifier()
			&& wheat==tp.getWheatModifier() && wool==tp.getWoolModifier();

}

TEST(Player_constructor){
	Player tp(NULL, "Buster");
	CHECK_EQUAL("Buster", tp.getName());
	CHECK_EQUAL(0, tp.getLongestRoad());
	CHECK_EQUAL(0, tp.getArmySize());
	CHECK_EQUAL(0, tp.getDevCardsInHand());

	CHECK(validateResourceAmount(0,0,0,0,0,tp));
	CHECK(validateTradeModifiers(4,4,4,4,4,tp));
}

TEST(Adders_Positive){
	Player tp(NULL, "Buster");
	tp.addBrick(1);
	tp.addWood(1);
	tp.addOre(1);
	tp.addWheat(1);
	tp.addWool(1);
	CHECK(validateResourceAmount(1,1,1,1,1,tp));
	tp.addMultiple(1,1,1,1,1);
	CHECK(validateResourceAmount(2,2,2,2,2,tp));
}

TEST(Adders_Negative_Normal){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	tp.addBrick(-1);
	tp.addWood(-1);
	tp.addOre(-1);
	tp.addWheat(-1);
	tp.addWool(-1);
	CHECK(validateResourceAmount(4,4,4,4,4,tp));
	tp.addMultiple(-1,-1,-1,-1,-1);
	CHECK(validateResourceAmount(3,3,3,3,3,tp));
}

TEST(Adders_Negative_Excessive){
	Player tp(NULL, "Buster");
	tp.addMultiple(1,1,1,1,1);
	tp.addBrick(-2);
	tp.addWood(-2);
	tp.addOre(-2);
	tp.addWheat(-2);
	tp.addWool(-2);
	CHECK(validateResourceAmount(0,0,0,0,0,tp));
	tp.addMultiple(1,1,1,1,1);
	tp.addMultiple(-2,-2,-2,-2,-2);
	CHECK(validateResourceAmount(0,0,0,0,0,tp));
}

//TRADE MODIFIERS
TEST(Trade_Modifiers_Brick){
	Player tp(NULL, "Buster");
	tp.setBrickModifier();
	CHECK(validateTradeModifiers(4,2,4,4,4,tp));
}

TEST(Trade_Modifiers_Wood){
	Player tp(NULL, "Buster");
	tp.setWoodModifier();
	CHECK(validateTradeModifiers(2,4,4,4,4,tp));
}

TEST(Trade_Modifiers_Ore){
	Player tp(NULL, "Buster");
	tp.setOreModifier();
	CHECK(validateTradeModifiers(4,4,2,4,4,tp));
}

TEST(Trade_Modifiers_Wheat){
	Player tp(NULL, "Buster");
	tp.setWheatModifier();
	CHECK(validateTradeModifiers(4,4,4,2,4,tp));
}

TEST(Trade_Modifiers_Wool){
	Player tp(NULL, "Buster");
	tp.setWoolModifier();
	CHECK(validateTradeModifiers(4,4,4,4,2,tp));
}

TEST(Trade_Modifiers_3){
	Player tp(NULL, "Buster");
	tp.setGeneralModifier();
	CHECK(validateTradeModifiers(3,3,3,3,3,tp));
}

TEST(Trade_Modifiers_Mixed){
	Player tp(NULL, "Buster");
	tp.setWheatModifier();
	tp.setOreModifier();
	tp.setGeneralModifier();
	CHECK(validateTradeModifiers(3,3,2,2,3,tp));
}

//PLAYER PURCHASES
TEST(Buy_Settlement_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	CHECK_EQUAL(true, tp.canBuySettlement());
	tp.buySettlement();
	CHECK(validateResourceAmount(4,4,5,4,4,tp));
}

TEST(Buy_Settlement_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(0,1,1,1,1);
	CHECK_EQUAL(false, tp.canBuySettlement());
	tp.buySettlement();
	CHECK(validateResourceAmount(0,1,1,1,1,tp));
}

TEST(Buy_Road_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	CHECK_EQUAL(true, tp.canBuyRoad());
	tp.buyRoad();
	CHECK(validateResourceAmount(4,4,5,5,5,tp));
}

TEST(Buy_Road_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(0,1,1,1,1);
	CHECK_EQUAL(false, tp.canBuyRoad());
	tp.buyRoad();
	CHECK(validateResourceAmount(0,1,1,1,1,tp));
}

TEST(Buy_City_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	CHECK_EQUAL(true, tp.canBuyCity());
	tp.buyCity();
	CHECK(validateResourceAmount(5,5,2,3,5,tp));
}

TEST(Buy_City_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(1,1,1,1,1);
	CHECK_EQUAL(false, tp.canBuyCity());
	tp.buyCity();
	CHECK(validateResourceAmount(1,1,1,1,1,tp));
}

TEST(Buy_Wonder_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(6,6,6,6,6);
	CHECK_EQUAL(true, tp.canBuyWonder());
	tp.buyWonder();
	CHECK(validateResourceAmount(1,1,1,1,1,tp));
}

TEST(Buy_Wonder_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,1,5,5);
	CHECK_EQUAL(false, tp.canBuyWonder());
	tp.buyWonder();
	CHECK(validateResourceAmount(5,5,1,5,5,tp));
}

TEST(Buy_DevCard_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	CHECK_EQUAL(true, tp.canBuyCard());
	tp.buyCard();
	CHECK(validateResourceAmount(5,5,4,4,4,tp));
}

TEST(Buy_DevCard_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(1,1,0,1,1);
	CHECK_EQUAL(false, tp.canBuyCard());
	tp.buyCard();
	CHECK(validateResourceAmount(1,1,0,1,1,tp));
}


//INSERT VICTORY POINT TESTS HERE


//INSERT DEV CARD TESTS HERE


//INSERT TRADING TESTS HERE


