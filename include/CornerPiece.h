#ifndef CORNERPIECE_H
#define CORNERPIECE_H

#include "GamePiece.h"
#include "Player.h"

class CornerPiece : public GamePiece {
private:
	Player& owner;
public:
	CornerPiece(GameBoard& board, Coordinate location, Player& owner);
	CornerPiece(CornerPiece&) = delete;
	~CornerPiece();
	CornerPiece& operator=(CornerPiece&) = delete;
	
	Player& getOwner();
	const Player& getOwner() const;

	virtual int getResourceModifier();

	virtual int getVictoryPoints();
};

#endif
