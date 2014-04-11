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
	CHECK_EQUAL(0, tp.getWood());
	CHECK_EQUAL(0, tp.getBrick());
	CHECK_EQUAL(0, tp.getOre());
	CHECK_EQUAL(0, tp.getWheat());
	CHECK_EQUAL(0, tp.getOre());
	CHECK_EQUAL(4, tp.getWoodModifier());
	CHECK_EQUAL(4, tp.getBrickModifier());
	CHECK_EQUAL(4, tp.getOreModifier());
	CHECK_EQUAL(4, tp.getWheatModifier());
	CHECK_EQUAL(4, tp.getOreModifier());
}

TEST(Adders_Positive){
	Player tp(NULL, "Buster");
	tp.addBrick(1);
	tp.addWood(1);
	tp.addOre(1);
	tp.addWheat(1);
	tp.addWool(1);
	CHECK_EQUAL(1, tp.getWood());
	CHECK_EQUAL(1, tp.getBrick());
	CHECK_EQUAL(1, tp.getOre());
	CHECK_EQUAL(1, tp.getWheat());
	CHECK_EQUAL(1, tp.getOre());
	tp.addMultiple(1,1,1,1,1);
	CHECK_EQUAL(2, tp.getWood());
	CHECK_EQUAL(2, tp.getBrick());
	CHECK_EQUAL(2, tp.getOre());
	CHECK_EQUAL(2, tp.getWheat());
	CHECK_EQUAL(2, tp.getOre());
}

TEST(Adders_Negative_Normal){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	tp.addBrick(-1);
	tp.addWood(-1);
	tp.addOre(-1);
	tp.addWheat(-1);
	tp.addWool(-1);
	CHECK_EQUAL(4, tp.getWood());
	CHECK_EQUAL(4, tp.getBrick());
	CHECK_EQUAL(4, tp.getOre());
	CHECK_EQUAL(4, tp.getWheat());
	CHECK_EQUAL(4, tp.getOre());
	tp.addMultiple(-1,-1,-1,-1,-1);
	CHECK_EQUAL(3, tp.getWood());
	CHECK_EQUAL(3, tp.getBrick());
	CHECK_EQUAL(3, tp.getOre());
	CHECK_EQUAL(3, tp.getWheat());
	CHECK_EQUAL(3, tp.getOre());
}

TEST(Adders_Negative_Excessive){
	Player tp(NULL, "Buster");
	tp.addMultiple(1,1,1,1,1);
	tp.addBrick(-2);
	tp.addWood(-2);
	tp.addOre(-2);
	tp.addWheat(-2);
	tp.addWool(-2);
	CHECK_EQUAL(0, tp.getWood());
	CHECK_EQUAL(0, tp.getBrick());
	CHECK_EQUAL(0, tp.getOre());
	CHECK_EQUAL(0, tp.getWheat());
	CHECK_EQUAL(0, tp.getOre());
	tp.addMultiple(1,1,1,1,1);
	tp.addMultiple(-2,-2,-2,-2,-2);
	CHECK_EQUAL(0, tp.getWood());
	CHECK_EQUAL(0, tp.getBrick());
	CHECK_EQUAL(0, tp.getOre());
	CHECK_EQUAL(0, tp.getWheat());
	CHECK_EQUAL(0, tp.getOre());
}

TEST(Trade_Modifiers_Brick){
	Player tp(NULL, "Buster");
	tp.setBrickModifier();
	CHECK_EQUAL(4, tp.getWoodModifier());
	CHECK_EQUAL(2, tp.getBrickModifier());
	CHECK_EQUAL(4, tp.getOreModifier());
	CHECK_EQUAL(4, tp.getWheatModifier());
	CHECK_EQUAL(4, tp.getOreModifier());
}

TEST(Trade_Modifiers_Wood){
	Player tp(NULL, "Buster");
	tp.setWoodModifier();
	CHECK_EQUAL(2, tp.getWoodModifier());
	CHECK_EQUAL(4, tp.getBrickModifier());
	CHECK_EQUAL(4, tp.getOreModifier());
	CHECK_EQUAL(4, tp.getWheatModifier());
	CHECK_EQUAL(4, tp.getOreModifier());
}

TEST(Trade_Modifiers_Ore){
	Player tp(NULL, "Buster");
	tp.setOreModifier();
	CHECK_EQUAL(4, tp.getWoodModifier());
	CHECK_EQUAL(4, tp.getBrickModifier());
	CHECK_EQUAL(2, tp.getOreModifier());
	CHECK_EQUAL(4, tp.getWheatModifier());
	CHECK_EQUAL(4, tp.getOreModifier());
}

TEST(Trade_Modifiers_Wheat){
	Player tp(NULL, "Buster");
	tp.setWheatModifier();
	CHECK_EQUAL(4, tp.getWoodModifier());
	CHECK_EQUAL(4, tp.getBrickModifier());
	CHECK_EQUAL(4, tp.getOreModifier());
	CHECK_EQUAL(2, tp.getWheatModifier());
	CHECK_EQUAL(4, tp.getOreModifier());
}

TEST(Trade_Modifiers_Wool){
	Player tp(NULL, "Buster");
	tp.setWoolModifier();
	CHECK_EQUAL(4, tp.getWoodModifier());
	CHECK_EQUAL(4, tp.getBrickModifier());
	CHECK_EQUAL(4, tp.getOreModifier());
	CHECK_EQUAL(4, tp.getWheatModifier());
	CHECK_EQUAL(2, tp.getOreModifier());
}

TEST(Trade_Modifiers_3){
	Player tp(NULL, "Buster");
	tp.setGeneralModifier();
	CHECK_EQUAL(3, tp.getWoodModifier());
	CHECK_EQUAL(3, tp.getBrickModifier());
	CHECK_EQUAL(3, tp.getOreModifier());
	CHECK_EQUAL(3, tp.getWheatModifier());
	CHECK_EQUAL(3, tp.getOreModifier());
}

TEST(Trade_Modifiers_Mixed){
	Player tp(NULL, "Buster");
	tp.setWheatModifier();
	tp.setOreModifier();
	tp.setGeneralModifier();
	CHECK_EQUAL(3, tp.getWoodModifier());
	CHECK_EQUAL(3, tp.getBrickModifier());
	CHECK_EQUAL(3, tp.getOreModifier());
	CHECK_EQUAL(2, tp.getWheatModifier());
	CHECK_EQUAL(2, tp.getOreModifier());
}

TEST(Buy_Settlement_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	CHECK_EQUAL(true, tp.canBuySettlement());
	tp.buySettlement();
	CHECK_EQUAL(4, tp.getWood());
	CHECK_EQUAL(4, tp.getBrick());
	CHECK_EQUAL(5, tp.getOre());
	CHECK_EQUAL(4, tp.getWheat());
	CHECK_EQUAL(4, tp.getOre());
}

TEST(Buy_Settlement_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(0,1,1,1,1);
	CHECK_EQUAL(false, tp.canBuySettlement());
	tp.buySettlement();
	CHECK_EQUAL(0, tp.getWood());
	CHECK_EQUAL(1, tp.getBrick());
	CHECK_EQUAL(1, tp.getOre());
	CHECK_EQUAL(1, tp.getWheat());
	CHECK_EQUAL(1, tp.getOre());
}

TEST(Buy_Road_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	CHECK_EQUAL(true, tp.canBuyRoad());
	tp.buyRoad();
	CHECK_EQUAL(4, tp.getWood());
	CHECK_EQUAL(4, tp.getBrick());
	CHECK_EQUAL(5, tp.getOre());
	CHECK_EQUAL(5, tp.getWheat());
	CHECK_EQUAL(5, tp.getOre());
}

TEST(Buy_Road_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(0,1,1,1,1);
	CHECK_EQUAL(false, tp.canBuyRoad());
	tp.buyRoad();
	CHECK_EQUAL(0, tp.getWood());
	CHECK_EQUAL(1, tp.getBrick());
	CHECK_EQUAL(1, tp.getOre());
	CHECK_EQUAL(1, tp.getWheat());
	CHECK_EQUAL(1, tp.getOre());
}

TEST(Buy_City_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	CHECK_EQUAL(true, tp.canBuyCity());
	tp.buyCity();
	CHECK_EQUAL(5, tp.getWood());
	CHECK_EQUAL(5, tp.getBrick());
	CHECK_EQUAL(2, tp.getOre());
	CHECK_EQUAL(3, tp.getWheat());
	CHECK_EQUAL(5, tp.getOre());
}

TEST(Buy_City_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(1,1,1,1,1);
	CHECK_EQUAL(false, tp.canBuyCity());
	tp.buyCity();
	CHECK_EQUAL(1, tp.getWood());
	CHECK_EQUAL(1, tp.getBrick());
	CHECK_EQUAL(1, tp.getOre());
	CHECK_EQUAL(1, tp.getWheat());
	CHECK_EQUAL(1, tp.getOre());
}

TEST(Buy_Wonder_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(6,6,6,6,6);
	CHECK_EQUAL(true, tp.canBuyWonder());
	tp.buyWonder();
	CHECK_EQUAL(1, tp.getWood());
	CHECK_EQUAL(1, tp.getBrick());
	CHECK_EQUAL(1, tp.getOre());
	CHECK_EQUAL(1, tp.getWheat());
	CHECK_EQUAL(1, tp.getOre());
}

TEST(Buy_Wonder_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,1,5,5);
	CHECK_EQUAL(false, tp.canBuyWonder());
	tp.buyWonder();
	CHECK_EQUAL(5, tp.getWood());
	CHECK_EQUAL(5, tp.getBrick());
	CHECK_EQUAL(1, tp.getOre());
	CHECK_EQUAL(5, tp.getWheat());
	CHECK_EQUAL(5, tp.getOre());
}

TEST(Buy_DevCard_True){
	Player tp(NULL, "Buster");
	tp.addMultiple(5,5,5,5,5);
	CHECK_EQUAL(true, tp.canBuyCard());
	tp.buyCard();
	CHECK_EQUAL(5, tp.getWood());
	CHECK_EQUAL(5, tp.getBrick());
	CHECK_EQUAL(4, tp.getOre());
	CHECK_EQUAL(4, tp.getWheat());
	CHECK_EQUAL(4, tp.getOre());
}

TEST(Buy_DevCard_False){
	Player tp(NULL, "Buster");
	tp.addMultiple(1,1,0,1,1);
	CHECK_EQUAL(false, tp.canBuyCard());
	tp.buyCard();
	CHECK_EQUAL(1, tp.getWood());
	CHECK_EQUAL(1, tp.getBrick());
	CHECK_EQUAL(0, tp.getOre());
	CHECK_EQUAL(1, tp.getWheat());
	CHECK_EQUAL(1, tp.getOre());
}


//INSERT VICTORY POINT TESTS HERE


//INSERT DEV CARD TESTS HERE


//INSERT TRADING TESTS HERE
