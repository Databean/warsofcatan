#ifndef CORNERPIECE_H
#define CORNERPIECE_H

class CornerPiece {
private:

public:
	CornerPiece();
	CornerPiece(CornerPiece&) = delete;
	~CornerPiece();
	CornerPiece& operator=(CornerPiece&) = delete;
};

#endif
