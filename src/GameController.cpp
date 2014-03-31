#include "GameController.h"

#include <iostream>

#include "GameBoard.h"
#include "GameView.h"
#include "Renderer.h"

GameController::GameController(GameBoard& model, GameView& view) : model(model), view(view), placingRoads(false), placingCities(false) ,lastCoordClick(-100, -100) {
	view.addElement(makeViewButtonColor([&](ScreenCoordinate coord) { 
		placingRoads = true; 
		placingCities = false;
		return true;
	}, {{0, 0}, {0.1, 0.1}}, std::make_tuple(1.f, 0.f, 0.f)));
	view.addElement(makeViewButtonColor([&](ScreenCoordinate coord) { 
		placingRoads = false; 
		placingCities = true;
		return true;
	}, {{0, 0.1}, {0.1, 0.2}}, std::make_tuple(0.f, 1.0f, 0.f)));
	view.addElement(makeViewButton([this](ScreenCoordinate coord) { this->handleEvent(ClickCoordinateEvent(screenToCoord(coord))); return true; }, {{0, 0}, {1, 1}}));
}

GameController::~GameController() {
	
}

void GameController::handleEvent(const ClickCoordinateEvent& event) {
	//std::cout << "user clicked at " << event.getCoordinate().first << ", " << event.getCoordinate().second << std::endl;
	/*
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
	}*/
	if(placingRoads) {
		if(lastCoordClick.first == -100 && lastCoordClick.second == -100) {
			lastCoordClick = event.getCoordinate();
		} else {
			model.PlaceRoad(lastCoordClick, event.getCoordinate(), *model.getPlayers()[0]);
			lastCoordClick = {-100, -100};
		}
	} else if(placingCities) {
		model.PlaceSettlement(event.getCoordinate(), *model.getPlayers()[0]);
	}
}

