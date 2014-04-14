#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Util.h"
#include <vector>

#include <array>

class GameBoard;
class ClickCoordinateEvent;
class GameView;
class Player;


enum ControlState {BASESTATE, MODALSTATE, BUILDROAD, BUILDSETTLEMENT, ROBBER,
	VICTORYPOINT_DEVCARD, BUILDROAD_DEVCARD, KNIGHT_DEVCARD, YEAROFPLENTY_DEVCARD, MONOPOLY_DEVCARD};


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
	bool handleCancelDialogueEvent(ScreenCoordinate);

	void pushState(ControlState);
	ControlState getState();
	ControlState popState();
bool handleTradeOffer(ScreenCoordinate, Player& initiating, std::array<int, 5>, Player& receiving, std::array<int, 5>);
	void storeClick(Coordinate clickCoordinate);
	Coordinate getLastClick();
	Coordinate getPastClick(int howLongAgo);
	void clearClickHistory();
	bool hasClickHistory();
	int getClickHistorySize();





	bool handlePlayerClick(ScreenCoordinate, Player&);
	bool handleTradeOffer(ScreenCoordinate, Player& initiating, std::array<int, 5>, Player& receiving, std::array<int, 5>);
};

#endif
