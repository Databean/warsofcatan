#ifndef CITY_H
#define CITY_H

#include "CornerPiece.h"

class City : public CornerPiece {
private:

public:
	City(GameBoard& board, Coordinate location, Player& owner);
	City(City&) = delete;
	~City();
	City& operator=(City&) = delete;
};

#endif
