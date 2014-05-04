#include <vector>
#include <memory>
#include <map>
#include <stdexcept>

#include "gtest/gtest.h"

#include "GameBoard.h"
#include "GamePiece.h"
#include "DevelopmentCard.h"
#include "Util.h"

void testBuyingCard(Player& test_player, std::unique_ptr<DevelopmentCard> card, bool correct_result){
	int prevOre = test_player.getOre();
	int prevWheat = test_player.getWheat();
	int prevWool = test_player.getWool();
	int prevCards = test_player.getDevelopmentCards(card->getType());
	ASSERT_EQ(test_player.buyCard(card), correct_result);
	if(correct_result){
		ASSERT_EQ(prevOre, test_player.getOre()+1);
		ASSERT_EQ(prevWheat, test_player.getWheat()+1);
		ASSERT_EQ(prevWool, test_player.getWool()+1);
		ASSERT_EQ(prevCards, test_player.getDevelopmentCards(card->getType()) -1);
	}else{
		ASSERT_EQ(prevOre, test_player.getOre());
		ASSERT_EQ(prevWheat, test_player.getWheat());
		ASSERT_EQ(prevWool, test_player.getWool());
		ASSERT_EQ(prevCards, test_player.getDevelopmentCards(card->getType()));
	}
}

TEST(DevCardTest, buying_card){
	GameBoard board({"tester"});
	Player& testPlayer = board.getPlayer(0);
	
	testPlayer.addOre(5);
	testPlayer.addWheat(5);
	testPlayer.addWool(5);
	
	testBuyingCard(testPlayer, std::unique_ptr<DevelopmentCard>(new KnightCard()), true);
	testBuyingCard(testPlayer, std::unique_ptr<DevelopmentCard>(new VictoryPointCard()), true);
	testBuyingCard(testPlayer, std::unique_ptr<DevelopmentCard>(new YearOfPlentyCard()), true);
	testBuyingCard(testPlayer, std::unique_ptr<DevelopmentCard>(new MonopolyCard()), true);
	testBuyingCard(testPlayer, std::unique_ptr<DevelopmentCard>(new RoadBuildingCard()), true);
	
	testBuyingCard(testPlayer, std::unique_ptr<DevelopmentCard>(new KnightCard()), false);
	testBuyingCard(testPlayer, std::unique_ptr<DevelopmentCard>(new VictoryPointCard()), false);
	testBuyingCard(testPlayer, std::unique_ptr<DevelopmentCard>(new YearOfPlentyCard()), false);
	testBuyingCard(testPlayer, std::unique_ptr<DevelopmentCard>(new MonopolyCard()), false);
	testBuyingCard(testPlayer, std::unique_ptr<DevelopmentCard>(new RoadBuildingCard()), false);
}

void testRoadBuildingCard(Player& test_player, bool correct_result, GameBoard& test_board, Coordinate start1, Coordinate end1, Coordinate start2, Coordinate end2){
	int prevCards = test_player.getRoadBuildingCards();
	
	ASSERT_EQ(test_player.playRoadBuilding(start1, end1, start2, end2), correct_result);
	Road * test_road1 = test_board.getRoad(start1, end1).get();
	Road * test_road2 = test_board.getRoad(start2, end2).get();
	if(correct_result){
		if (test_road1 == NULL || test_road2 == NULL){
			ASSERT_TRUE(false);
		}else{
			ASSERT_TRUE(test_road1->equals(start1, end1));
			ASSERT_TRUE(test_road2->equals(start2, end2));
			ASSERT_EQ(prevCards, (test_player.getRoadBuildingCards() + 1));
		}
	}else{
		if(test_road1 == NULL && test_road2 == NULL){
			ASSERT_EQ(prevCards, test_player.getRoadBuildingCards());
		}else{
			ASSERT_TRUE(false);
		}
	}
}

TEST(DevCardTest, RoadBuildingCard){

	GameBoard test_board({"tester1"});
	Player& test_player = *test_board.getPlayers()[0];

	test_board.PlaceSettlement(Coordinate(0,0), test_player);
	std::unique_ptr<DevelopmentCard> test_card = std::unique_ptr<DevelopmentCard>(new RoadBuildingCard());

	test_player.addOre(4);
	test_player.addWheat(4);
	test_player.addWool(4);
	test_player.buyCard(test_card);
	test_player.buyCard(test_card);
	test_player.buyCard(test_card);
	test_player.buyCard(test_card);

	testRoadBuildingCard(test_player, true, test_board, Coordinate(0,0), Coordinate(-1,1), Coordinate(-1,1), Coordinate(-1,2));
	testRoadBuildingCard(test_player, true, test_board, Coordinate(0,0), Coordinate(1,0), Coordinate(1,0), Coordinate(1,1));
	testRoadBuildingCard(test_player, true, test_board, Coordinate(0,2), Coordinate(1,1), Coordinate(1,1), Coordinate(2,1));
	testRoadBuildingCard(test_player, true, test_board, Coordinate(1,3), Coordinate(0,3), Coordinate(0,3), Coordinate(0,2));

	testRoadBuildingCard(test_player, false, test_board, Coordinate(-500,200), Coordinate(100,340), Coordinate(21123,12312), Coordinate(343,321));
}

void testVictoryPointCard(Player & test_player, bool correct_result){
	int prevCards = test_player.getVictoryCards();

	ASSERT_EQ(test_player.playVictoryCard(), correct_result);
	if(correct_result){
		ASSERT_EQ(prevCards, test_player.getVictoryCards()+1);
	}else{
		ASSERT_EQ(prevCards, test_player.getVictoryCards());
	}

}


TEST(DevCardTest, VictoryPointCard){
	GameBoard test_board({"tester1"});
	Player& test_player = *test_board.getPlayers()[0];

	std::unique_ptr<DevelopmentCard> test_card = std::unique_ptr<DevelopmentCard>(new VictoryPointCard());

	test_player.addOre(2);
	test_player.addWheat(2);
	test_player.addWool(2);
	test_player.buyCard(test_card);
	test_player.buyCard(test_card);

	testVictoryPointCard(test_player, true);
	testVictoryPointCard(test_player, true);

	testVictoryPointCard(test_player, false);
}

void testMonopolyCard(Player& test_player, bool correct_result, int resource_type, int expected_gain){
	int prevCard = test_player.getMonopolyCards();int prevReso = -1;
	try{
		prevReso = test_player.getResource(resource_type);
	}catch (std::runtime_error & e){
		ASSERT_TRUE(resource_type >= 5);
		return;
	}
	
	ASSERT_EQ(test_player.playMonopoly(resource_type), correct_result);
	if(correct_result){
		ASSERT_EQ(prevCard, test_player.getMonopolyCards()+1);
		ASSERT_EQ(prevReso, test_player.getResource(resource_type)-expected_gain);
	}else{
		ASSERT_EQ(prevCard, test_player.getMonopolyCards());
		ASSERT_EQ(prevReso, test_player.getResource(resource_type));
	}
}


TEST(DevCardTest, MonopolyCard){
	GameBoard test_board({"tester1", "tester2", "tester3"});
	Player& test_player1 = test_board.getPlayer(0);
	Player& test_player2 = test_board.getPlayer(1);
	Player& test_player3 = test_board.getPlayer(2);

	std::unique_ptr<DevelopmentCard> test_card = std::unique_ptr<DevelopmentCard>(new MonopolyCard());

	test_player1.addOre(3);
	test_player1.addWheat(3);
	test_player1.addWool(3);
	test_player1.buyCard(test_card);
	test_player1.buyCard(test_card);
	test_player1.buyCard(test_card);

	testMonopolyCard(test_player1, true, ORE_INDEX, 0);

	test_player2.addWool(7);
	test_player3.addWool(28);
	testMonopolyCard(test_player1, true, WOOL_INDEX, 35);

	test_player2.addWood(5);
	test_player3.addOre(10);
	testMonopolyCard(test_player1, false, 300, 0);
	testMonopolyCard(test_player1, true, WOOD_INDEX, 5);

	testMonopolyCard(test_player1, false, WHEAT_INDEX, 0);
}


void testYearOfPlenty(Player& test_player, bool correct_result, int resource_type){
	int prevCard = test_player.getYearOfPlentyCards();
	int prevReso = -1;
	try{
		prevReso = test_player.getResource(resource_type);
	}catch (std::runtime_error & e){
		ASSERT_TRUE(resource_type >= 5);
		return;
	}
	
	ASSERT_EQ(test_player.playYearOfPlenty(resource_type), correct_result);
	if(correct_result){
		ASSERT_EQ(prevCard, test_player.getYearOfPlentyCards()+1);
		ASSERT_EQ(prevReso, test_player.getResource(resource_type)-2);
	}else{
		ASSERT_EQ(prevCard, test_player.getYearOfPlentyCards());
		ASSERT_EQ(prevReso, test_player.getResource(resource_type));
	}
}

TEST(DevCardTest, YearOfPlentyCard){
	GameBoard test_board({"tester1"});
	Player& test_player = test_board.getPlayer(0);

	std::unique_ptr<DevelopmentCard> test_card = std::unique_ptr<YearOfPlentyCard>(new YearOfPlentyCard());

	test_player.addOre(2);
	test_player.addWheat(2);
	test_player.addWool(2);
	test_player.buyCard(test_card);
	test_player.buyCard(test_card);

	testYearOfPlenty(test_player, true, ORE_INDEX);
	testYearOfPlenty(test_player, false, 300);
	testYearOfPlenty(test_player, true, WOOD_INDEX);
	testYearOfPlenty(test_player, false, WOOL_INDEX);
}


int getResourceSum(Player& player){
	int sum = 0;
	for(int i = 0; i < 5; i++){
		sum += player.getResource(i);
	}
	return sum;
}

void testKnightCard(Player& test_player, bool correct_result, GameBoard& test_board, Coordinate newLocation, Player& opponent, int gain_expected){
	Coordinate prevRobber = test_board.getRobber();
	int prevArmy = test_player.getArmySize();
	int prevCards = test_player.getKnightCards();
	int player_prevResourceSum = getResourceSum(test_player);
	int opponent_prevResourceSum = getResourceSum(opponent);
	
	ASSERT_EQ(test_player.playKnight(newLocation, opponent), correct_result);
	if(correct_result){
		ASSERT_EQ(newLocation, test_board.getRobber());
		ASSERT_EQ(prevArmy+1, test_player.getArmySize());
		ASSERT_EQ(prevCards-1, test_player.getKnightCards());
		ASSERT_EQ(player_prevResourceSum+gain_expected, getResourceSum(test_player));
		ASSERT_EQ(opponent_prevResourceSum-gain_expected, getResourceSum(opponent));
	}else{
		ASSERT_EQ(prevRobber, test_board.getRobber());
		ASSERT_EQ(prevArmy, test_player.getArmySize());
		ASSERT_EQ(prevCards, test_player.getKnightCards());
		ASSERT_EQ(player_prevResourceSum, getResourceSum(test_player));
		ASSERT_EQ(opponent_prevResourceSum, getResourceSum(opponent));
	}


}


TEST(DevCardTest, KnightCard){
	GameBoard test_board({"tester1", "tester2", "tester3"});
	Player& test_player1 = test_board.getPlayer(0);
	Player& test_player2 = test_board.getPlayer(1);
	Player& test_player3 = test_board.getPlayer(2);

	std::unique_ptr<DevelopmentCard> test_card = std::unique_ptr<DevelopmentCard>(new KnightCard());

	test_player1.addOre(4);
	test_player1.addWheat(4);
	test_player1.addWool(4);
	test_player1.buyCard(test_card);
	test_player1.buyCard(test_card);
	test_player1.buyCard(test_card);
	test_player1.buyCard(test_card);

	test_board.moveRobber(Coordinate(0,1));

	testKnightCard(test_player1, false, test_board, Coordinate(0,4), test_player2, 0);
	test_player2.addWood(1);
	test_board.PlaceSettlement(Coordinate(0,3), test_player2);
	testKnightCard(test_player1, true, test_board, Coordinate(0,4), test_player1, 0);
	testKnightCard(test_player1, true, test_board, Coordinate(0,4), test_player2, 1);

	test_player3.addWood(1);
	test_player3.addOre(1);
	test_board.PlaceSettlement(Coordinate(0,0), test_player3);
	testKnightCard(test_player1, true, test_board, Coordinate(0,1), test_player3, 1);

	testKnightCard(test_player1, true, test_board, Coordinate(0,4), test_player2, 0);

	testKnightCard(test_player1, false, test_board, Coordinate(0,4), test_player3, 0);
}














