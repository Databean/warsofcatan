#ifndef SETTLEMENT_H
#define SETTLEMENT_H

class Settlement: public CornerPiece {
private:

public:
	Settlement();
	Settlement(Settlement&) = delete;
	~Settlement();
	Settlement& operator=(Settlement&) = delete;
};

#endif
