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
	
	auto font = "resources/TypeWritersSubstitute-Black.ttf";
	auto fontSize = 50;
	
	view.addElement(makeViewButtonText(std::bind(&GameController::handleRoadButtonEvent, this, _1), {{0, 0}, {0.1, 0.1}}, font, fontSize, "Road"));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleSettlementButtonEvent, this, _1), {{0, 0.1}, {0.1, 0.2}}, font, fontSize, "Settlement"));
	view.addElement(makeViewButton(std::bind(&GameController::handleBoardEvent, this, _1), {{0, 0}, {1, 1}}));
}

/**
 * Destroy the GameController.
 */
GameController::~GameController() {
	
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

