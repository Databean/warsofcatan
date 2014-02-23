#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <map>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include <cstddef>

#include "Util.h"
#include "GamePiece.h"

class GameBoard {
private:
	std::map<Coordinate, std::unique_ptr<GamePiece>> pieces;
	std::vector<std::unique_ptr<const Road>> roads;

	int constructBoardFromFile(std::ifstream &file);
	int constructFileFromBoard(std::ofstream &file);

public:
	GameBoard();
	GameBoard(GameBoard&) = delete;
	~GameBoard();
	GameBoard& operator=(GameBoard&) = delete;

	int save_Board(std::string filename);
	int load_Board(std::string filename);
};

#endif
