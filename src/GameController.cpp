#include "GameController.h"

#include <iostream>
#include <functional>
#include <memory>

#include "GameBoard.h"
#include "GameView.h"
#include "Renderer.h"
#include "Player.h"

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
	
	auto playerTopY = 0.9;
	for(auto i = 0; i < model.getNoOfPlayers(); i++) {
		auto width = 0.2;
		Player& player = model.getPlayer(i);
		view.addElement(makeViewButtonText(std::bind(&GameController::handlePlayerClick, this, _1, std::ref(player)), {{1.0 - width, playerTopY - 0.1}, {1.0, playerTopY}}, font, fontSize, player.getName()));
		playerTopY -= 0.05;
	}
	
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

template<int size>
auto negativeArr(std::array<int, size> arr) -> std::array<int, size> {
	for(auto& it : arr) {
		it = -it;
	}
	return arr;
}

/**
 * Handles a click on one of the Player names at the top right of the screen.
 * @param coord The coordinate clicked on.
 * @param player The player whose name was clicked on.
 */
bool GameController::handlePlayerClick(ScreenCoordinate coord, Player& player) {
	using namespace std::placeholders;
	Player& initiating = *model.getPlayers()[0];
	Player& receiving = player;
	auto priority = -10;
	
	std::array<int, 5> initial{{0, 0, 0, 0, 0}};
	
	//std::function<bool(std::array<int, 5>, ScreenCoordinate)> tradeFunction(std::bind(&GameController::handleTradeOffer, this, _2, std::ref(initiating), _1, std::ref(receiving)));
	std::function<bool(std::array<int, 5>, ScreenCoordinate)> tradeFunction([this, &initiating, &receiving](std::array<int, 5> offer, ScreenCoordinate coord) {
		std::array<int, 5> initial{{0, 0, 0, 0, 0}};
		std::array<int, 5> reverseOffer = negativeArr<5>(offer);
		handleTradeOffer(coord, receiving, initial, initiating, reverseOffer);
		return true;
	});
	std::function<bool(ScreenCoordinate)> cancelFunction([this, priority](ScreenCoordinate coord) {
		view.removeElement(priority); 
		return true;
	});
	
	view.addElement(priority, std::unique_ptr<ViewElement>(new TradingView(initiating, receiving, tradeFunction, cancelFunction, initial)));
	std::cout << player.getName() << std::endl;
	return true;
}

/**
 * Handle a trade offer from a player.
 * @param coord The coordinate clicked on to initiate the trade.
 * @param initiating The player initiating the trade.
 * @param offer The offer the player is giving.
 * @param receiving The other player in the trade.
 */
bool GameController::handleTradeOffer(ScreenCoordinate coord, Player& initiating, std::array<int, 5> offer, Player& receiving, std::array<int, 5> counterOffer) {
	auto priority = -10;
	if(offer == negativeArr<5>(counterOffer)) {
		view.removeElement(priority);
		//TODO: perform trade
	} else {
		//std::function<bool(std::array<int, 5>, ScreenCoordinate)> tradeFunction(std::bind(&GameController::handleTradeOffer, this, _2, std::ref(initiating), _1, std::ref(receiving)));
		std::function<bool(std::array<int, 5>, ScreenCoordinate)> tradeFunction([this, &initiating, &receiving, counterOffer](std::array<int, 5> offer, ScreenCoordinate coord) {
			std::array<int, 5> reverseOffer = negativeArr<5>(offer);
			handleTradeOffer(coord, receiving, counterOffer, initiating, reverseOffer);
			return true;
		});
		std::function<bool(ScreenCoordinate)> cancelFunction([this, priority](ScreenCoordinate coord) {
			view.removeElement(priority); 
			return true;
		});
		
		view.addElement(priority, std::unique_ptr<ViewElement>(new TradingView(initiating, receiving, tradeFunction, cancelFunction, counterOffer)));
	}
	return true;
}

