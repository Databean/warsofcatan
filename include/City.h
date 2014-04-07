#ifndef CITY_H
#define CITY_H

#include "CornerPiece.h"

/**
 * Upgraded from a Settlement. Exists on the board, and receives two resources from adjacent tiles when their number is rolled.
 */
class City : public CornerPiece {
private:

public:
	City(GameBoard& board, Coordinate location, Player& owner);
	City(City&) = delete;
	~City();
	City(CornerPiece& sett);
	City& operator=(City&) = delete;
	
	virtual void accept(GameVisitor&);
	virtual bool operator==(const GamePiece& piece) const;

	int getResourceModifier();
	int getVictoryPoints();
};

#endif
