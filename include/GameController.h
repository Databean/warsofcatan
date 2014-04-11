#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Util.h"
#include <vector>

class GameBoard;
class ClickCoordinateEvent;
class GameView;


enum ControlState {BASESTATE, MODALSTATE, BUILDROAD, BUILDSETTLEMENT, ROBBER, BUILDROAD_DEVCARD, KNIGHT_DEVCARD, YEAROFPLENTY_DEVCARD, MONOPOLY_DEVCARD};


/**
 * Takes interpreted Catan events from the View and calls the appropriate functions on the model to change the state
 * in response.
 */
class GameController {
private:
	GameBoard& model;
	GameView& view;
	
	std::vector<ControlState> stateStack;
	std::vector<Coordinate> clickHistory;
	
	GameController(const GameController& o) : model(o.model), view(o.view) {} //deleted
	GameController& operator=(const GameController& o) { return *this; } //deleted
public:
	GameController(GameBoard&, GameView& view);
	~GameController();
	
	bool handleBoardEvent(ScreenCoordinate);
	bool handleRoadButtonEvent(ScreenCoordinate);
	bool handleSettlementButtonEvent(ScreenCoordinate);
	bool handleRoadCardButtonEvent(ScreenCoordinate);
	bool handleKnightCardButtonEvent(ScreenCoordinate);
	bool handleYearOfPlentyCardButtonEvent(ScreenCoordinate);
	bool handleMonopolyCardButtonEvent(ScreenCoordinate);
	bool handleVictoryPointCardButtonEvent(ScreenCoordinate);
	bool handleCancelButtonEvent(ScreenCoordinate);

	bool handleConfirmRoadCard(ScreenCoordinate);
	bool handleCancelRoadCard(ScreenCoordinate);

	void pushState(ControlState);
	ControlState getState();
	ControlState popState();

	void storeClick(Coordinate clickCoordinate);
	Coordinate getLastClick();
	Coordinate getPastClick(int howLongAgo);
	void clearClickHistory();
	bool hasClickHistory();
	int getClickHistorySize();





};

#endif
