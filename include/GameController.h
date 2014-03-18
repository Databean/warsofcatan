#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

class GameBoard;
class ClickCoordinateEvent;

class GameController {
private:
	GameBoard& model;
	
	GameController(const GameController& o) : model(o.model) {} //deleted
	GameController& operator=(const GameController& o) { return *this; } //deleted
public:
	GameController(GameBoard&);
	~GameController();
	
	void handleEvent(const ClickCoordinateEvent&);
};

#endif