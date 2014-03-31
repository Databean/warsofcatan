#include "GameController.h"

#include <iostream>
#include <functional>

#include "GameBoard.h"
#include "GameView.h"
#include "Renderer.h"

GameController::GameController(GameBoard& model, GameView& view) : model(model), view(view), placingRoads(false), placingCities(false) ,lastCoordClick(-100, -100) {
	using namespace std::placeholders;
	
	view.addElement(makeViewButtonColor(std::bind(&GameController::handleRoadButtonEvent, this, _1), {{0, 0}, {0.1, 0.1}}, std::make_tuple(1.f, 0.f, 0.f)));
	view.addElement(makeViewButtonColor(std::bind(&GameController::handleSettlementButtonEvent, this, _1), {{0, 0.1}, {0.1, 0.2}}, std::make_tuple(0.f, 1.0f, 0.f)));
	view.addElement(makeViewButton(std::bind(&GameController::handleBoardEvent, this, _1), {{0, 0}, {1, 1}}));
}

GameController::~GameController() {
	
}

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

bool GameController::handleRoadButtonEvent(ScreenCoordinate coord) {
	placingRoads = true; 
	placingCities = false;
	return true;
}

bool GameController::handleSettlementButtonEvent(ScreenCoordinate coord) {
	placingRoads = false; 
	placingCities = true;
	return true;
}

