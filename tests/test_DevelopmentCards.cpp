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
	std::vector<std::unique_ptr<Player>> players {};
	players.emplace_back(new Player("tester"));
	Player& test_player = *players[0];
	GameBoard test_board(std::move(players));
	test_player.setBoard(&test_board);

	test_board.PlaceSettlement(Coordinate(0,0), test_player);

	RoadBuildingCard test_card(&test_player);

	try{
		test_card.playCard(Coordinate(0,0), Coordinate(-1,1), Coordinate(0,0), Coordinate(1,0));
		CHECK(true);
	} catch (std::invalid_argument& e){
		std::cout << e.what();
		CHECK(false);
	}
}

TEST(RoadBuildingCard_bad){
	std::vector<std::unique_ptr<Player>> players {};
	players.emplace_back(new Player("tester"));
	Player& test_player = *players[0];
	GameBoard test_board(std::move(players));
	test_player.setBoard(&test_board);

	test_board.PlaceSettlement(Coordinate(0,0), test_player);

	RoadBuildingCard test_card(&test_player);

	try{
		test_card.playCard(Coordinate(0,0), Coordinate(0,2), Coordinate(0,0), Coordinate(1,0));
		CHECK(false);
	} catch (std::invalid_argument& e){
		CHECK(true);
	}

	try{
		test_card.playCard(Coordinate(0,0), Coordinate(0,1), Coordinate(0,0), Coordinate(2,2));
		CHECK(false);
	} catch (std::invalid_argument& e){
		CHECK(true);
	}

}














