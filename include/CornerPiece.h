#ifndef CORNERPIECE_H
#define CORNERPIECE_H

#include "GamePiece.h"
#include "Player.h"

/**
 * A piece that exists on one of the corners of a resource hexagon. Currently is either a Settlement or City, both of which
 * receive resources from adjacent resource tiles.
 */
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

	void accept(GameVisitor& visitor)

	virtual int getResourceModifier();

	virtual int getVictoryPoints();

	virtual bool operator==(const GamePiece&) const;
};

#endif
