#include <vector>
#include <memory>
#include <map>
#include <stdexcept>

#include "GameBoard.h"
#include "GamePiece.h"
#include "DevelopmentCard.h"
#include "Util.h"
#include "UnitTest++.h"

void testBuyingCard(Player& test_player, std::unique_ptr<DevelopmentCard> card, bool correct_result){
	int prevOre = test_player.getOre();
	int prevWheat = test_player.getWheat();
	int prevWool = test_player.getWool();
	int prevCards = test_player.getDevelopmentCards(card->getType());
	CHECK(test_player.buyCard(card) == correct_result);
	if(correct_result){
		CHECK(prevOre == test_player.getOre()+1);
		CHECK(prevWheat == test_player.getWheat()+1);
		CHECK(prevWool == test_player.getWool()+1);
		CHECK(prevCards == test_player.getDevelopmentCards(card->getType()) -1);
	}else{
		CHECK(prevOre == test_player.getOre());
		CHECK(prevWheat == test_player.getWheat());
		CHECK(prevWool == test_player.getWool());
		CHECK(prevCards == test_player.getDevelopmentCards(card->getType()));
	}

}


TEST(buying_card){
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
	std::cout
				<< "S1:" << start1.first<< "," << start1.second
				<< " E1:" << end1.first<< "," << end1.second
				<< " S2:" << start2.first<< "," << start2.second
				<< " E2:" << end2.first<< "," << end2.second << "\n";

	CHECK(test_player.playRoadBuilding(start1, end1, start2, end2) ==  correct_result);

	Road * test_road1 = test_board.getRoad(start1, end1).get();
	Road * test_road2 = test_board.getRoad(start2, end2).get();
	if(correct_result){
		if (test_road1 == NULL || test_road2 == NULL){
			CHECK(false);
		}else{
			CHECK(test_road1->equals(start1, end1));
			CHECK(test_road2->equals(start2, end2));
			CHECK(prevCards == (test_player.getRoadBuildingCards() + 1));
		}
	}else{
		if(test_road1 == NULL && test_road2 == NULL){
			CHECK(prevCards == test_player.getRoadBuildingCards());
		}else{
			CHECK(false);
		}
	}
}

TEST(RoadBuildingCard){

	GameBoard test_board({"tester1"});
	Player& test_player = *test_board.getPlayers()[0];

	test_board.PlaceSettlement(Coordinate(0,0), test_player);
	std::unique_ptr<DevelopmentCard> test_card = std::unique_ptr<DevelopmentCard>(new RoadBuildingCard());

	test_player.addOre(3);
	test_player.addWheat(3);
	test_player.addWool(3);
	test_player.buyCard(test_card);
	test_player.buyCard(test_card);
	test_player.buyCard(test_card);


	testRoadBuildingCard(test_player, true, test_board, Coordinate(0,0), Coordinate(1,0), Coordinate(1,0), Coordinate(1,1));
	testRoadBuildingCard(test_player, true, test_board, Coordinate(0,2), Coordinate(1,1), Coordinate(-1,1), Coordinate(0,0));
	testRoadBuildingCard(test_player, true, test_board, Coordinate(1,3), Coordinate(0,3), Coordinate(0,3), Coordinate(0,2));

	testRoadBuildingCard(test_player, false, test_board, Coordinate(-500,200), Coordinate(100,340), Coordinate(21123,12312), Coordinate(343,321));
}

void testVictoryPointCard(Player & test_player, bool correct_result){
	int prevPoints = test_player.getVictoryPointsWithoutCards();
	int prevCards = test_player.getVictoryCards();

	CHECK(test_player.playVictoryCard() == correct_result);
	if(correct_result){
		CHECK(prevPoints == test_player.getVictoryPointsWithoutCards() -1);
		CHECK(prevCards == test_player.getVictoryCards()+1);
	}else{
		CHECK(prevPoints == test_player.getVictoryPointsWithoutCards());
		CHECK(prevCards == test_player.getVictoryCards());
	}

}


TEST(VictoryPointCard){
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



















