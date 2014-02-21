#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <map>
#include <vector>
#include <memory>
#include <utility>

#include "Util.h"
#include "GamePiece.h"

class GameBoard {
private:
	std::map<Coordinate, std::unique_ptr<GamePiece>> pieces;
	std::vector<std::unique_ptr<const Road>> roads;
public:
	GameBoard();
	GameBoard(GameBoard&) = delete;
	~GameBoard();
	GameBoard& operator=(GameBoard&) = delete;
	
	const std::map<Coordinate, std::unique_ptr<GamePiece>>& getPieces() const;
};

#endif