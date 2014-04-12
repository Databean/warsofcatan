#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Util.h"

class GameBoard;
class ClickCoordinateEvent;
class GameView;

/**
 * Takes interpreted Catan events from the View and calls the appropriate functions on the model to changee the state
 * in response.
 */
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
	
	void nextTurn(ScreenCoordinate);

	bool handleBoardEvent(ScreenCoordinate);
	bool handleRoadButtonEvent(ScreenCoordinate);
	bool handleSettlementButtonEvent(ScreenCoordinate);
};

#endif
