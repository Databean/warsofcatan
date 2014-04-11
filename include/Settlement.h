#ifndef SETTLEMENT_H
#define SETTLEMENT_H

#include "CornerPiece.h"

/**
 * Exists on the board at a location and is owned by a player. Receives one resource when an adjacent resource tile
 * has its number rolled on the dice. Worth one victory point.
 */
class Settlement : public CornerPiece {
private:

public:
	Settlement(GameBoard& board, Coordinate location, Player& owner);
	Settlement(Settlement&) = delete;
	~Settlement();
	Settlement& operator=(Settlement&) = delete;
	
	virtual void accept(GameVisitor&);
	virtual bool operator==(const GamePiece&) const;
	int getResourceModifier();
	int getVictoryPoints();

	bool operator==(const Settlement&) const;
};

#endif
