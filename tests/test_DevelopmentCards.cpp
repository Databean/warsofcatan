#include <vector>
#include <memory>
#include <map>
#include <stdexcept>

#include "GameBoard.h"
#include "GamePiece.h"
#include "DevelopmentCard.h"
#include "Util.h"
#include "UnitTest++.h"


TEST(RoadBuildingCard_good){
	GameBoard test_board({"tester"});
	Player& test_player = test_board.getPlayer(0);

	test_board.PlaceSettlement(Coordinate(0,0), test_player);

	RoadBuildingCard test_card(test_board);

	try{
		test_card.playCard(&test_player, Coordinate(0,0), Coordinate(-1,1), Coordinate(0,0), Coordinate(1,0));
		CHECK(true);
	} catch (std::invalid_argument& e){
		std::cout << e.what();
		CHECK(false);
	}
}

TEST(RoadBuildingCard_bad){
	GameBoard test_board({"tester"});
	Player& test_player = test_board.getPlayer(0);

	test_board.PlaceSettlement(Coordinate(0,0), test_player);

	RoadBuildingCard test_card(test_board);

	try{
		test_card.playCard(&test_player, Coordinate(0,0), Coordinate(0,2), Coordinate(0,0), Coordinate(1,0));
		CHECK(false);
	} catch (std::invalid_argument& e){
		CHECK(true);
	}

	try{
		test_card.playCard(&test_player, Coordinate(0,0), Coordinate(0,1), Coordinate(0,0), Coordinate(2,2));
		CHECK(false);
	} catch (std::invalid_argument& e){
		CHECK(true);
	}

}














