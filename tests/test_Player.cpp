/*
 * test_Player&Purchase.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: Kyle Grage
 */

#include "gtest/gtest.h"

#include "Player.h"
#include "GameBoard.h"


/**
 * Check to see if a player's resources are equal to the input
 * @param [resource]x5 the amount of (wood, brick, ore, wheat, wool) you are checking
 * @param player
 * @return bool if the values match
 */
bool validateResourceAmount(int wood, int brick, int ore, int wheat, int wool, Player& tp){
	return wood==tp.getWood() && brick==tp.getBrick() && ore==tp.getOre() && wheat==tp.getWheat() && wool==tp.getWool();
}

/**
 * Check to see if a player's trade modifiers are equal to the input
 * @param [resource]x5 the modifiers (wood, brick, ore, wheat, wool) you are checking
 * @param player
 * @return bool if the values match
 */
bool validateTradeModifiers(int wood, int brick, int ore, int wheat, int wool, Player& tp){
	return wood==tp.getWoodModifier() && brick==tp.getBrickModifier() && ore==tp.getOreModifier()
			&& wheat==tp.getWheatModifier() && wool==tp.getWoolModifier();

}

TEST(PlayerTest, Player_constructor){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	ASSERT_EQ(0, tp.getLongestRoadSize());
	ASSERT_EQ(0, tp.getArmySize());
	ASSERT_EQ(0, tp.getDevCardsInHand());

	ASSERT_TRUE(validateResourceAmount(0,0,0,0,0,tp));
	ASSERT_TRUE(validateTradeModifiers(4,4,4,4,4,tp));
}

TEST(PlayerTest, Adders_Positive){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addBrick(1);
	tp.addWood(1);
	tp.addOre(1);
	tp.addWheat(1);
	tp.addWool(1);
	ASSERT_TRUE(validateResourceAmount(1,1,1,1,1,tp));
	tp.addMultiple(1,1,1,1,1);
	ASSERT_TRUE(validateResourceAmount(2,2,2,2,2,tp));
}

TEST(PlayerTest, Adders_Negative_Normal){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addMultiple(5,5,5,5,5);
	tp.addBrick(-1);
	tp.addWood(-1);
	tp.addOre(-1);
	tp.addWheat(-1);
	tp.addWool(-1);
	ASSERT_TRUE(validateResourceAmount(4,4,4,4,4,tp));
	tp.addMultiple(-1,-1,-1,-1,-1);
	ASSERT_TRUE(validateResourceAmount(3,3,3,3,3,tp));
}

TEST(PlayerTest, Adders_Negative_Excessive){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addMultiple(1,1,1,1,1);
	tp.addBrick(-2);
	tp.addWood(-2);
	tp.addOre(-2);
	tp.addWheat(-2);
	tp.addWool(-2);
	ASSERT_TRUE(validateResourceAmount(0,0,0,0,0,tp));
	tp.addMultiple(1,1,1,1,1);
	tp.addMultiple(-2,-2,-2,-2,-2);
	ASSERT_TRUE(validateResourceAmount(0,0,0,0,0,tp));
}

//TRADE MODIFIERS
TEST(PlayerTest, Trade_Modifiers_Brick){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.setBrickModifier();
	ASSERT_TRUE(validateTradeModifiers(4,2,4,4,4,tp));
}

TEST(PlayerTest, Trade_Modifiers_Wood){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.setWoodModifier();
	ASSERT_TRUE(validateTradeModifiers(2,4,4,4,4,tp));
}

TEST(PlayerTest, Trade_Modifiers_Ore){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.setOreModifier();
	ASSERT_TRUE(validateTradeModifiers(4,4,2,4,4,tp));
}

TEST(PlayerTest, Trade_Modifiers_Wheat){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.setWheatModifier();
	ASSERT_TRUE(validateTradeModifiers(4,4,4,2,4,tp));
}

TEST(PlayerTest, Trade_Modifiers_Wool){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.setWoolModifier();
	ASSERT_TRUE(validateTradeModifiers(4,4,4,4,2,tp));
}

TEST(PlayerTest, Trade_Modifiers_3){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.setGeneralModifier();
	ASSERT_TRUE(validateTradeModifiers(3,3,3,3,3,tp));
}

TEST(PlayerTest, Trade_Modifiers_Mixed){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.setWheatModifier();
	tp.setOreModifier();
	tp.setGeneralModifier();
	ASSERT_TRUE(validateTradeModifiers(3,3,2,2,3,tp));
}

//PLAYER PURCHASES
TEST(PlayerTest, Buy_Settlement_True){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addMultiple(5,5,5,5,5);
	ASSERT_EQ(true, tp.canBuySettlement());
	tp.buySettlement();
	ASSERT_TRUE(validateResourceAmount(4,4,5,4,4,tp));
}

TEST(PlayerTest, Buy_Settlement_False){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addMultiple(0,1,1,1,1);
	ASSERT_EQ(false, tp.canBuySettlement());
	tp.buySettlement();
	ASSERT_TRUE(validateResourceAmount(0,1,1,1,1,tp));
}

TEST(PlayerTest, Buy_Road_True){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addMultiple(5,5,5,5,5);
	ASSERT_EQ(true, tp.canBuyRoad());
	tp.buyRoad();
	ASSERT_TRUE(validateResourceAmount(4,4,5,5,5,tp));
}

TEST(PlayerTest, Buy_Road_False){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addMultiple(0,1,1,1,1);
	ASSERT_EQ(false, tp.canBuyRoad());
	tp.buyRoad();
	ASSERT_TRUE(validateResourceAmount(0,1,1,1,1,tp));
}

TEST(PlayerTest, Buy_City_True){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addMultiple(5,5,5,5,5);
	ASSERT_EQ(true, tp.canBuyCity());
	tp.buyCity();
	ASSERT_TRUE(validateResourceAmount(5,5,2,3,5,tp));
}

TEST(PlayerTest, Buy_City_False){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addMultiple(1,1,1,1,1);
	ASSERT_EQ(false, tp.canBuyCity());
	tp.buyCity();
	ASSERT_TRUE(validateResourceAmount(1,1,1,1,1,tp));
}

TEST(PlayerTest, Buy_Wonder_True){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addMultiple(6,6,6,6,6);
	ASSERT_EQ(true, tp.canBuyWonder());
	tp.buyWonder();
	ASSERT_TRUE(validateResourceAmount(1,1,1,1,1,tp));
}

TEST(PlayerTest, Buy_Wonder_False){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addMultiple(5,5,1,5,5);
	ASSERT_EQ(false, tp.canBuyWonder());
	tp.buyWonder();
	ASSERT_TRUE(validateResourceAmount(5,5,1,5,5,tp));
}

TEST(PlayerTest, Buy_DevCard_True){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addMultiple(5,5,5,5,5);
	ASSERT_EQ(true, tp.canBuyCard());
	tp.buyCard();
	ASSERT_TRUE(validateResourceAmount(5,5,4,4,4,tp));
}

TEST(PlayerTest, Buy_DevCard_False){
	GameBoard board({"test board"});
	Player& tp = board.getPlayer(0);
	tp.addMultiple(1,1,0,1,1);
	ASSERT_EQ(false, tp.canBuyCard());
	tp.buyCard();
	ASSERT_TRUE(validateResourceAmount(1,1,0,1,1,tp));
}


//INSERT VICTORY POINT TESTS HERE
TEST(PlayerTest, UpdateVictoryPoints_Settlements){
	GameBoard test_board({"tester"});
	Player& test_player = test_board.getPlayer(0);

	ASSERT_EQ(test_player.getVictoryPoints(), 0);

	Coordinate start1(0,0);
	test_board.PlaceSettlement(start1, test_player);

	ASSERT_EQ(test_player.getVictoryPoints(), 1);
}

TEST(PlayerTest, UpdateVictoryPoints_cities){
	GameBoard test_board({"tester"});
	Player& test_player = test_board.getPlayer(0);

	ASSERT_EQ(test_player.getVictoryPoints(), 0);

	Coordinate start1(0,0);
	test_board.PlaceSettlement(start1, test_player);

	ASSERT_EQ(test_player.getVictoryPoints(), 1);

	test_board.UpgradeSettlement(start1);
	ASSERT_EQ(test_player.getVictoryPoints(), 2);
}

TEST(PlayerTest, UpdateVictoryPoints_longestRoad){
	GameBoard test_board({"tester"});
	Player& test_player = test_board.getPlayer(0);

	ASSERT_EQ(test_player.getVictoryPoints(), 0);

	Coordinate start1(0,0);
	test_board.PlaceSettlement(start1, test_player);

	ASSERT_EQ(test_player.getVictoryPoints(), 1);

	test_board.PlaceRoad(start1, Coordinate(-1,1), test_player);
	test_board.PlaceRoad(Coordinate(-1,1), Coordinate(-1,2), test_player);
	test_board.PlaceRoad(Coordinate(-1,2), Coordinate(0,2), test_player);
	test_board.PlaceRoad(Coordinate(0,2), Coordinate(0,3), test_player);
	test_board.PlaceRoad(Coordinate(0,3), Coordinate(1,3), test_player);

	ASSERT_EQ(test_player.getVictoryPoints(), 3);
}

TEST(PlayerTest, UpdateVictoryPoints_VictoryCards){
	GameBoard test_board({"tester"});
	Player& test_player = test_board.getPlayer(0);

	ASSERT_EQ(test_player.getVictoryPoints(), 0);

	std::unique_ptr<DevelopmentCard> test_VictoryCard = std::unique_ptr<DevelopmentCard>(new VictoryPointCard());
	test_player.addOre(1);
	test_player.addWheat(1);
	test_player.addWool(1);

	test_player.buyCard(test_VictoryCard);
	ASSERT_EQ(test_player.getVictoryPoints(), 1);
}

TEST(PlayerTest, UpdateVictoryPoints_LargestArmy){
	GameBoard test_board({"tester1", "tester2"});
	Player& test_player = test_board.getPlayer(0);
	Player& opponent = test_board.getPlayer(1);

	ASSERT_EQ(test_player.getVictoryPoints(), 0);

	test_player.addOre(3);
	test_player.addWheat(3);
	test_player.addWool(3);

	std::unique_ptr<DevelopmentCard> test_KnightCard = std::unique_ptr<DevelopmentCard>(new KnightCard());
	test_player.buyCard(test_KnightCard);
	test_player.buyCard(test_KnightCard);
	test_player.buyCard(test_KnightCard);

	test_board.PlaceSettlement(Coordinate(0,0), opponent);

	test_player.playKnight(Coordinate(0,1), opponent);
	test_player.playKnight(Coordinate(0,1), opponent);
	test_player.playKnight(Coordinate(0,1), opponent);

	ASSERT_EQ(test_player.getVictoryPoints(), 2);
}

TEST(PlayerTest, UpdateVictoryPoints_all){
	GameBoard test_board({"tester", "opponent"});
	Player& test_player = test_board.getPlayer(0);
	Player& opponent = test_board.getPlayer(1);

	ASSERT_EQ(test_player.getVictoryPoints(), 0);

	Coordinate start1(0,0);

	test_board.PlaceSettlement(start1, test_player);

	ASSERT_EQ(test_player.getVictoryPoints(), 1);

	test_board.PlaceRoad(start1, Coordinate(-1,1), test_player);
	test_board.PlaceRoad(Coordinate(-1,1), Coordinate(-1,2), test_player);
	test_board.PlaceRoad(Coordinate(-1,2), Coordinate(0,2), test_player);
	test_board.PlaceRoad(Coordinate(0,2), Coordinate(0,3), test_player);
	test_board.PlaceRoad(Coordinate(0,3), Coordinate(1,3), test_player);

	ASSERT_EQ(test_player.getVictoryPoints(), 3);

	test_board.UpgradeSettlement(start1);

	ASSERT_EQ(test_player.getVictoryPoints(), 4);

	std::unique_ptr<DevelopmentCard> test_VictoryCard = std::unique_ptr<DevelopmentCard>(new VictoryPointCard());
	test_player.addOre(4);
	test_player.addWheat(4);
	test_player.addWool(4);

	test_player.buyCard(test_VictoryCard);
	ASSERT_EQ(test_player.getVictoryPoints(), 5);

	std::unique_ptr<DevelopmentCard> test_KnightCard = std::unique_ptr<DevelopmentCard>(new KnightCard());
	test_player.buyCard(test_KnightCard);
	test_player.buyCard(test_KnightCard);
	test_player.buyCard(test_KnightCard);

	test_board.PlaceSettlement(Coordinate(0,5), opponent);

	test_player.playKnight(Coordinate(0,4), opponent);
	test_player.playKnight(Coordinate(0,4), opponent);
	test_player.playKnight(Coordinate(0,4), opponent);

	ASSERT_EQ(test_player.getVictoryPoints(), 7);
}


//INSERT DEV CARD TESTS HERE

TEST(PlayerTest, Monopoly_card_1){
	GameBoard board({"test-player1", "test-player2"});

	Player& tp1 = board.getPlayer(0);
	Player& tp2 = board.getPlayer(1);

	tp1.addMultiple(0,0,1,1,1);

	std::unique_ptr<DevelopmentCard> test_MonopolyCard = std::unique_ptr<DevelopmentCard>(new MonopolyCard());
	tp1.buyCard(test_MonopolyCard);


	tp1.addMultiple(5,5,5,5,5);
	tp2.addMultiple(5,5,5,5,5);

	tp1.playMonopoly(2);

	ASSERT_TRUE(validateResourceAmount(5,5,10,5,5,tp1));
	ASSERT_TRUE(validateResourceAmount(5,5,0,5,5,tp2));

}


TEST(PlayerTest, Monopoly_card_2){
	GameBoard board({"test-player1", "test-player2"});

	Player& tp1 = board.getPlayer(0);
	Player& tp2 = board.getPlayer(1);

	tp1.addMultiple(0,0,1,1,1);

	std::unique_ptr<DevelopmentCard> test_MonopolyCard = std::unique_ptr<DevelopmentCard>(new MonopolyCard());
	tp1.buyCard(test_MonopolyCard);

	tp1.addMultiple(0,0,0,1,1);
	tp2.addMultiple(1,1,0,0,0);

	tp1.playMonopoly(2);


	ASSERT_TRUE(validateResourceAmount(0,0,0,1,1,tp1));
	ASSERT_TRUE(validateResourceAmount(1,1,0,0,0,tp2));

}



TEST(PlayerTest, Monopoly_card_false){
	GameBoard board({"test-player1", "test-player2"});

	Player& tp1 = board.getPlayer(0);
	Player& tp2 = board.getPlayer(1);

	std::unique_ptr<DevelopmentCard> test_MonopolyCard = std::unique_ptr<DevelopmentCard>(new MonopolyCard());
	tp1.buyCard(test_MonopolyCard);

	tp1.addMultiple(0,0,0,1,1);
	tp2.addMultiple(1,1,0,0,0);

	tp1.playMonopoly(0);

	ASSERT_TRUE(validateResourceAmount(0,0,0,1,1,tp1));
	ASSERT_TRUE(validateResourceAmount(1,1,0,0,0,tp2));

}


TEST(PlayerTest, Monopoly_card_false_2){
	GameBoard board({"test-player1", "test-player2"});

	Player& tp1 = board.getPlayer(0);
	Player& tp2 = board.getPlayer(1);

	tp1.addMultiple(0,0,0,1,1);
	tp2.addMultiple(1,1,0,0,0);

	tp1.playMonopoly(0);

	ASSERT_TRUE(validateResourceAmount(0,0,0,1,1,tp1));
	ASSERT_TRUE(validateResourceAmount(1,1,0,0,0,tp2));

}


TEST(PlayerTest, Year_Of_Plenty_card){
	GameBoard board({"test-player1"});

	Player& tp1 = board.getPlayer(0);

	tp1.addMultiple(0,0,1,1,1);

	std::unique_ptr<DevelopmentCard> test_YearOfPlentyCard = std::unique_ptr<DevelopmentCard>(new YearOfPlentyCard());
	tp1.buyCard(test_YearOfPlentyCard);

	tp1.addMultiple(0,0,0,1,1);

	tp1.playYearOfPlenty(2);

	ASSERT_TRUE(validateResourceAmount(0,0,2,1,1,tp1));

}



TEST(PlayerTest, Year_Of_Plenty_card_false){
	GameBoard board({"test-player1"});

	Player& tp1 = board.getPlayer(0);

	std::unique_ptr<DevelopmentCard> test_YearOfPlentyCard = std::unique_ptr<DevelopmentCard>(new YearOfPlentyCard());
	tp1.buyCard(test_YearOfPlentyCard);

	tp1.addMultiple(0,0,0,1,1);

	tp1.playYearOfPlenty(2);

	ASSERT_TRUE(validateResourceAmount(0,0,0,1,1,tp1));

}


TEST(PlayerTest, Year_Of_Plenty_card_false_2){
	GameBoard board({"test-player1"});

	Player& tp1 = board.getPlayer(0);

	tp1.addMultiple(0,0,0,1,1);

	tp1.playYearOfPlenty(2);

	ASSERT_TRUE(validateResourceAmount(0,0,0,1,1,tp1));

}



//INSERT TRADING TESTS HERE

TEST(PlayerTest, Trade_positive){
	GameBoard board({"test-player1", "test-player2"});

	Player& tp1 = board.getPlayer(0);
	Player& tp2 = board.getPlayer(1);

	tp1.addMultiple(5,5,5,5,5);
	tp2.addMultiple(5,5,5,5,5);

	std::array<int, 5> offer = {{2,2,0,0,0}};
	std::array<int, 5> demand = {{0,0,0,2,2}};

	tp2.acceptOffer(tp1, offer, demand);

	ASSERT_TRUE(validateResourceAmount(7,7,5,3,3,tp1));
	ASSERT_TRUE(validateResourceAmount(3,3,5,7,7,tp2));

}


TEST(PlayerTest, Trade_negative){
	GameBoard board({"test-player1", "test-player2"});

	Player& tp1 = board.getPlayer(0);
	Player& tp2 = board.getPlayer(1);

	tp1.addMultiple(2,2,2,2,2);
	tp2.addMultiple(2,2,2,2,2);

	std::array<int, 5> offer = {{5,5,0,0,0}};
	std::array<int, 5> demand = {{0,0,0,5,5}};

	tp2.acceptOffer(tp1, offer, demand);

	ASSERT_TRUE(validateResourceAmount(2,2,2,2,2,tp1));
	ASSERT_TRUE(validateResourceAmount(2,2,2,2,2,tp2));

}

