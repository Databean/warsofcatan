#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

class GameBoard;

class GameController {
private:
	GameBoard& board;
	
	GameController(const GameController& o) : board(o.board) {} //deleted
	GameController& operator=(const GameController& o) { return *this; } //deleted
public:
	GameController(GameBoard&);
	~GameController();
};

#endif