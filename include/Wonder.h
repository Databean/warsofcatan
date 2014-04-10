/*
 * Wonder.h
 *
 *  Created on: Apr 10, 2014
 *      Author: Kyle Grage
 */

#ifndef WONDER_H_
#define WONDER_H_

#include "CornerPiece.h"

class Wonder {
	private:

	public:
		Wonder(GameBoard& board, Coordinate location, Player& owner);
		Wonder(Wonder&) = delete;
		~Wonder();
		Wonder(CornerPiece& sett);
		Wonder& operator=(City&) = delete;

		virtual void accept(GameVisitor&);
		virtual bool operator==(const GamePiece& piece) const;

		int getResourceModifier();
		int getVictoryPoints();
};

#endif /* WONDER_H_ */
