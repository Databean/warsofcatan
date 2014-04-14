#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Util.h"

#include <array>

class GameBoard;
class ClickCoordinateEvent;
class GameView;
class Player;

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
	
	bool handleBoardEvent(ScreenCoordinate);
	bool handleRoadButtonEvent(ScreenCoordinate);
	bool handleSettlementButtonEvent(ScreenCoordinate);
	bool handlePlayerClick(ScreenCoordinate, Player&);
	bool handleTradeOffer(ScreenCoordinate, Player& initiating, std::array<int, 5>, Player& receiving, std::array<int, 5>);
};

#endif