#ifndef SETTLEMENT_H
#define SETTLEMENT_H

#include "CornerPiece.h"

class Settlement : public CornerPiece {
private:

public:
	Settlement(GameBoard& board, Coordinate location);
	Settlement(Settlement&) = delete;
	~Settlement();
	Settlement& operator=(Settlement&) = delete;
};

#endif
