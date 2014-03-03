#ifndef SETTLEMENT_H
#define SETTLEMENT_H

#include "CornerPiece.h"

class Settlement : public CornerPiece {
private:

public:
	Settlement(GameBoard& board, Coordinate location, Player& owner);
	Settlement(Settlement&) = delete;
	~Settlement();
	Settlement& operator=(Settlement&) = delete;
	
	virtual void accept(GameVisitor&);
	virtual bool operator==(const GamePiece&) const;
};

#endif
