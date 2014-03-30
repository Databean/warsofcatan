#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Util.h"

class GameBoard;
class ClickCoordinateEvent;
class GameView;

class GameController {
private:
	GameBoard& model;
	GameView& view;
	
	bool placingRoads;
	bool placingCities;
	Coordinate lastCoordClick;
	
	GameController(const GameController& o) : model(o.model), view(o.view) {} //deleted
	GameController& operator=(const GameController& o) { return *this; } //deleted
public:
	GameController(GameBoard&, GameView& view);
	~GameController();
	
	void handleEvent(const ClickCoordinateEvent&);
};

#endif