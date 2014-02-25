#ifndef CORNERPIECE_H
#define CORNERPIECE_H

#include "GamePiece.h"

class CornerPiece : public GamePiece {
private:

public:
	CornerPiece(GameBoard& board, Coordinate location);
	CornerPiece(CornerPiece&) = delete;
	~CornerPiece();
	CornerPiece& operator=(CornerPiece&) = delete;
};

#endif
