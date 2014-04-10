#include <vector>
#include <memory>
#include <map>
#include <stdexcept>

#include "gtest/gtest.h"

#include "GameBoard.h"
#include "GamePiece.h"
#include "DevelopmentCard.h"
#include "Util.h"


TEST(DevCardTest, RoadBuildingCard_good){
	GameBoard test_board({"tester"});
	Player& test_player = test_board.getPlayer(0);

	test_board.PlaceSettlement(Coordinate(0,0), test_player);

	RoadBuildingCard test_card(test_board);

	try{
		test_card.playCard(&test_player, Coordinate(0,0), Coordinate(-1,1), Coordinate(0,0), Coordinate(1,0));
		ASSERT_TRUE(true);
	} catch (std::invalid_argument& e){
		std::cout << e.what();
		ASSERT_TRUE(false);
	}
}

TEST(DevCardTest, RoadBuildingCard_bad){
	GameBoard test_board({"tester"});
	Player& test_player = test_board.getPlayer(0);

	test_board.PlaceSettlement(Coordinate(0,0), test_player);

	RoadBuildingCard test_card(test_board);

	try{
		test_card.playCard(&test_player, Coordinate(0,0), Coordinate(0,2), Coordinate(0,0), Coordinate(1,0));
		ASSERT_TRUE(false);
	} catch (std::invalid_argument& e){
		ASSERT_TRUE(true);
	}

	try{
		test_card.playCard(&test_player, Coordinate(0,0), Coordinate(0,1), Coordinate(0,0), Coordinate(2,2));
		ASSERT_TRUE(false);
	} catch (std::invalid_argument& e){
		ASSERT_TRUE(true);
	}

}














