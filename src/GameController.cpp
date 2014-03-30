#include "GameController.h"

#include <iostream>

#include "GameBoard.h"
#include "GameView.h"
#include "Renderer.h"

GameController::GameController(GameBoard& model, GameView& view) : model(model), view(view) {
	view.addElement(makeViewButton([this](ScreenCoordinate coord) { this->handleEvent(ClickCoordinateEvent(screenToCoord(coord))); return true; }, {{0, 0}, {1, 1}}));
}

GameController::~GameController() {
	
}

void GameController::handleEvent(const ClickCoordinateEvent& event) {
	//std::cout << "user clicked at " << event.getCoordinate().first << ", " << event.getCoordinate().second << std::endl;
	if(model.getRoads(event.getCoordinate()).size() > 0) {
		model.PlaceSettlement(event.getCoordinate(), *model.getPlayers()[0]);
	} else {
		static Coordinate adjacentCoordDiffs[] = {Coordinate(0, 1), Coordinate(1, 0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0), Coordinate(-1, 1)};
		for(auto& adjacent : adjacentCoordDiffs) {
			Coordinate start {event.getCoordinate().first + adjacent.first, event.getCoordinate().second + adjacent.second}; 
			if(model.getRoads(start).size() > 0) {
				model.PlaceRoad(start, event.getCoordinate(), *model.getPlayers()[0]);
				break;
			}
		}
	}
}

