#include "GameController.h"

#include <iostream>
#include <functional>

#include "GameBoard.h"
#include "GameView.h"
#include "Renderer.h"

/**
 * Initialize the game controller. Adds the buttons for user control to the view, binding them to GameController methods.
 * @param model The GameBoard model that is drawn to the screen.
 * @param view The GameView that is used to draw the model.
 */
GameController::GameController(GameBoard& model, GameView& view) : model(model), view(view), placingRoads(false), placingCities(false) ,lastCoordClick(-100, -100) {
	using namespace std::placeholders;
	
	view.addElement(makeViewButtonColor(std::bind(&GameController::nextTurn, this, _1), {{0, 0.2}, {0.1, 0.3}}, std::make_tuple(0.f, 0.f, 1.f)));
	view.addElement(makeViewButtonColor(std::bind(&GameController::handleRoadButtonEvent, this, _1), {{0, 0}, {0.1, 0.1}}, std::make_tuple(1.f, 0.f, 0.f)));
	view.addElement(makeViewButtonColor(std::bind(&GameController::handleSettlementButtonEvent, this, _1), {{0, 0.1}, {0.1, 0.2}}, std::make_tuple(0.f, 1.0f, 0.f)));
	view.addElement(makeViewButton(std::bind(&GameController::handleBoardEvent, this, _1), {{0, 0}, {1, 1}}));
}

/**
 * Destroy the GameController.
 */
GameController::~GameController() {
	
}

/**
 *  calls a function to advance turn, check for victory and roll dice
 */
void GameController::nextTurn(ScreenCoordinate) {
	model.endTurn();
}


/**
 * Handles a click that is actually on the tiles of the board. Either constructs a road or a settlement based on the control buttons the user has clicked.
 * @param screenCoord Where the user clicked on screen.
 * @return Whether this event was handled by this element. Always true.
 */
bool GameController::handleBoardEvent(ScreenCoordinate screenCoord) {
	auto coord = screenToCoord(screenCoord);
	if(placingRoads) {
		if(lastCoordClick.first == -100 && lastCoordClick.second == -100) {
			lastCoordClick = coord;
		} else {
			model.PlaceRoad(lastCoordClick, coord, *model.getPlayers()[0]);
			lastCoordClick = {-100, -100};
		}
	} else if(placingCities) {
		model.PlaceSettlement(coord, *model.getPlayers()[0]);
	}
	return true;
}

/**
 * Handles a click on the "create road" button. Changes the internal state to indicate the user is going to be making roads on the board.
 * @param coord The place the user clicked on screen
 * @return Whether this event was handled by this element. Always true.
 */
bool GameController::handleRoadButtonEvent(ScreenCoordinate coord) {
	placingRoads = true; 
	placingCities = false;
	return true;
}

/**
 * Handles a click on the "create settlement" button. Changes the internal state to indicate the user is going to be making roads on the board.
 * @param coord The place the user clicked on screen.
 * @return Whether thi sevent was handled by this element. Always true.
 */
bool GameController::handleSettlementButtonEvent(ScreenCoordinate coord) {
	placingRoads = false; 
	placingCities = true;
	return true;
}

