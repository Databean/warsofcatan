#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Util.h"
#include <vector>

#include <array>

class GameBoard;
class ClickCoordinateEvent;
class GameView;
class Player;


enum ControlState {BASESTATE, MODALSTATE, BUILDROAD, BUILDSETTLEMENT, BUILDCITY, BUILDWONDER, ROBBER,
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
	
	bool nextTurn(ScreenCoordinate);
	bool handleBoardEvent(ScreenCoordinate);
	bool handleRoadButtonEvent(ScreenCoordinate);
	bool handleSettlementButtonEvent(ScreenCoordinate);
	bool handleCityButtonEvent(ScreenCoordinate);
	bool handleWonderButtonEvent(ScreenCoordinate);
	bool handleRoadCardButtonEvent(ScreenCoordinate);

	bool handleBuyDevelopmentCardButtonEvent(ScreenCoordinate);
	bool handleKnightCardButtonEvent(ScreenCoordinate);
	bool handleYearOfPlentyCardButtonEvent(ScreenCoordinate);
	bool handleMonopolyCardButtonEvent(ScreenCoordinate);
	bool handleVictoryPointCardButtonEvent(ScreenCoordinate);

	bool handleWoodButtonEvent(ScreenCoordinate);
	bool handleSheepButtonEvent(ScreenCoordinate);
	bool handleWheatButtonEvent(ScreenCoordinate);
	bool handleOreButtonEvent(ScreenCoordinate);
	bool handleBrickButtonEvent(ScreenCoordinate);

	bool handleCancelButtonEvent(ScreenCoordinate);

	bool handleConfirmRoadCard(ScreenCoordinate);
	bool handleCancelDialogueEvent(ScreenCoordinate);
    
    bool viewCardTotals(ScreenCoordinate coord);

	void pushState(ControlState);
	ControlState getState();
	ControlState popState();
	void storeClick(Coordinate clickCoordinate);
	Coordinate getLastClick();
	Coordinate getPastClick(unsigned int howLongAgo);
	void clearClickHistory();
	bool hasClickHistory();
	int getClickHistorySize();

	void robPlayers();

	bool handlePlayerClick(ScreenCoordinate, Player&);
	bool handleTradeOffer(ScreenCoordinate, Player& initiating, std::array<int, 5>, Player& receiving, std::array<int, 5>);
	
	bool handleBankClick(ScreenCoordinate);
};

#endif
