#include "GameBoard.h"

#define ADD_RESOURCE(x, y, res, val) (resources[Coordinate(x,y)] = \
std::unique_ptr<GamePiece>(new ResourceTile(*this, Coordinate(x,y), res, val)))
#define DUMMY_BOARD //define to instantiate dummy board for debugging 

GameBoard::GameBoard() {

	init_resources();


}

GameBoard::~GameBoard() {
	
}
/*
std::vector<GamePiece> GameBoard::GetNeighbors(Coordinate location){
	std::vector<GamePiece> v;
	for (int i = -1; i < 1; i++)
	{
		Settlement* sett = dynamic_cast<Settlement*>(&(corners[Coordinate(location.first + i, location.second)]));
		if (sett != 0)
		{
			v.insert(sett)
		}
		Settlement sett = dynamic_cast<Settlement*>(corners[Coordinate(location.first + i, location.second - i)]);
		if (sett != 0)
		{
			v.insert(sett)		}
		Settlement sett = dynamic_cast<Settlement*>(corners[Coordinate(location.first, location.second - i)]);
		if (sett != 0)
		{
			v.insert(sett)
		}
	}
}

&/

/* initialize board with a set of resources. Right now, just creates a static tile arrangement to test
URL: http://images.fanpop.com/images/image_uploads/Differents-Boards-settlers-of-catan-521934_1157_768.jpg*/

void GameBoard::init_resources()
{
	

	#ifdef DUMMY_BOARD
	ADD_RESOURCE(0, 1, BRICK, 2);
	ADD_RESOURCE(-2, 2, SHEEP, 5);
	ADD_RESOURCE(2, 0, WOOD, 6);
	ADD_RESOURCE(-3, 3, DESERT, 0);
	ADD_RESOURCE(-1, 3, SHEEP, 10);
	ADD_RESOURCE(1, 2, WHEAT, 9);
	ADD_RESOURCE(3, 1, WHEAT, 3);
	ADD_RESOURCE(-4, 6, WOOD, 8);
	ADD_RESOURCE(-2, 5, SHEEP, 3);
	ADD_RESOURCE(0, 4, STONE, 11);
	ADD_RESOURCE(2, 3, STONE, 4);
	ADD_RESOURCE(4, 2, SHEEP, 8);
	ADD_RESOURCE(-3, 7, BRICK, 4);
	ADD_RESOURCE(-1, 6, WHEAT, 6);
	ADD_RESOURCE(1, 5, WOOD, 5);
	ADD_RESOURCE(3, 4, BRICK, 10);
	ADD_RESOURCE(-2, 8, WOOD, 11);
	ADD_RESOURCE(0, 7, STONE, 12);
	ADD_RESOURCE(2, 6, WHEAT, 9);
	#endif
	

}

void GameBoard::PlaceSettlement(Coordinate location, Player& Owner){
	corners[location] = std::unique_ptr<GamePiece>(new Settlement(*this, location, Owner));
}