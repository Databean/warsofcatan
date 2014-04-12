//#include <UnitTest++.h>
//
//#include "Serialization.h"
//#include "GameBoard.h"
//#include "DevelopmentCard.h"
//#include "Player.h"
//
//#include "tinyxml2.h"
//
//#include <iostream>
//#include <sstream>
//#include <memory>
//#include <vector>
//
//using std::vector;
//using std::unique_ptr;
//using std::stringstream;
//
//TEST(emptyBoardSerialization) {
//	vector<unique_ptr<Player>> players;
//
//	GameBoard testBoard(std::move(players));
//
//	stringstream stream;
//	testBoard.save(stream);
//
//	GameBoard copyBoard(stream);
//
//	CHECK(testBoard == copyBoard);
//}
//
//TEST(multiplePlayerSerialization) {
//	vector<unique_ptr<Player>> players;
//	players.emplace_back(unique_ptr<Player>(new Player("test")));
//	players.emplace_back(unique_ptr<Player>(new Player("test2")));
//
//	GameBoard testBoard(std::move(players));
//
//	stringstream stream;
//	testBoard.save(stream);
//
//	GameBoard copyBoard(stream);
//
//	CHECK(testBoard == copyBoard);
//}
//
//TEST(testCardSerialization) {
//	vector<unique_ptr<Player>> players;
//	players.emplace_back(unique_ptr<Player>(new Player("test")));
//
//
//	unique_ptr<DevelopmentCard> knight = unique_ptr<DevelopmentCard>(new KnightCard());
//	unique_ptr<DevelopmentCard> victory = unique_ptr<DevelopmentCard>(new VictoryPointCard());
//	unique_ptr<DevelopmentCard> yearofplenty = unique_ptr<DevelopmentCard>(new YearOfPlentyCard());
//	unique_ptr<DevelopmentCard> monopoly = unique_ptr<DevelopmentCard>(new MonopolyCard());
//	unique_ptr<DevelopmentCard> roadbuilding = unique_ptr<DevelopmentCard>(new RoadBuildingCard());
//
//	players[0]->buyCard(knight);
//	players[0]->buyCard(victory);
//	players[0]->buyCard(yearofplenty);
//	players[0]->buyCard(monopoly);
//	players[0]->buyCard(roadbuilding);
//
//	GameBoard testBoard(std::move(players));
//
//	stringstream stream;
//	testBoard.save(stream);
//
//	GameBoard copyBoard(stream);
//
//	CHECK(testBoard == copyBoard);
//}
//
//TEST(roadSerialization) {
//	vector<unique_ptr<Player>> players;
//	players.emplace_back(unique_ptr<Player>(new Player("test")));
//	players.emplace_back(unique_ptr<Player>(new Player("test2")));
//	std::cout << __LINE__ << "\n";
//	Player& firstPlayer = *players[0];
//	Player& secondPlayer = *players[1];
//	std::cout << __LINE__ << "\n";
//
//	GameBoard testBoard(std::move(players));
//
//	testBoard.PlaceSettlement(Coordinate(0,0), firstPlayer);
//	testBoard.PlaceSettlement(Coordinate(-1,1), secondPlayer);
//	testBoard.PlaceRoad(Coordinate(0,0), Coordinate(-1,1), firstPlayer);
//	testBoard.PlaceRoad(Coordinate(-1,1), Coordinate(-1,2), secondPlayer);
//
//	stringstream stream;
//	testBoard.save(stream);
//
//	GameBoard copyBoard(stream);
//
//	CHECK(testBoard == copyBoard);
//}
//
//TEST(settlementSerialization) {
//	vector<unique_ptr<Player>> players;
//	players.emplace_back(unique_ptr<Player>(new Player("test")));
//
//	Player& player = *players[0];
//
//	GameBoard testBoard(std::move(players));
//
//	testBoard.PlaceSettlement(Coordinate(0, 0), player);
//	testBoard.PlaceSettlement(Coordinate(1, 1), player);
//
//	stringstream stream;
//	testBoard.save(stream);
//
//	GameBoard copyBoard(stream);
//
//	CHECK(testBoard == copyBoard);
//}
