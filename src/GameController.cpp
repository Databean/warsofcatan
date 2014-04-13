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
GameController::GameController(GameBoard& model, GameView& view) : model(model), view(view) {
	using namespace std::placeholders;
	
	view.addElement(makeViewButton(std::bind(&GameController::handleBoardEvent, this, _1), {{0, 0}, {1, 1}}));

	view.addElement(makeViewButtonColor(std::bind(&GameController::handleRoadButtonEvent, this, _1), {{0, 0}, {0.1, 0.1}}, std::make_tuple(1.f, 0.f, 0.f)));
	view.addElement(makeViewButtonColor(std::bind(&GameController::handleSettlementButtonEvent, this, _1), {{0, 0.1}, {0.1, 0.2}}, std::make_tuple(0.f, 1.0f, 0.f)));


	view.addElement(makeViewButtonColor(std::bind(&GameController::handleCancelButtonEvent, this, _1), {{.95, .95}, {1.0, 1.0}}, std::make_tuple(1.f, 0.0f, 0.f)));

	view.addElement(makeViewButtonColor(std::bind(&GameController::handleRoadCardButtonEvent, this, _1), {{.2, 0}, {.3, .05}}, std::make_tuple(1.f, 0.0f, 0.f)));
	view.addElement(makeViewButtonColor(std::bind(&GameController::handleKnightCardButtonEvent, this, _1), {{0.3, 0}, {.4, .05}}, std::make_tuple(0.f, 0.0f, 0.f)));
	view.addElement(makeViewButtonColor(std::bind(&GameController::handleYearOfPlentyCardButtonEvent, this, _1), {{0.4, 0}, {.5, .05}}, std::make_tuple(1.f, 1.0f, 0.f)));
	view.addElement(makeViewButtonColor(std::bind(&GameController::handleMonopolyCardButtonEvent, this, _1), {{0.5, 0}, {.6, .05}}, std::make_tuple(1.f, 0.0f, 1.f)));
	view.addElement(makeViewButtonColor(std::bind(&GameController::handleVictoryPointCardButtonEvent, this, _1), {{0.6, 0}, {.7, .05}}, std::make_tuple(0.f, 1.0f, 1.f)));


	stateStack.push_back(BASESTATE);
}

/**
 * Destroy the GameController.
 */
GameController::~GameController() {
	
}

void GameController::pushState(ControlState newState){
	if (newState != BASESTATE){
		stateStack.push_back(newState);
	}
}

ControlState GameController::popState(){
	ControlState currState = getState();
	if(currState == BASESTATE){
		return currState;
	}
	stateStack.pop_back();
	return currState;
}

ControlState GameController::getState(){
	return stateStack.back();
}

void GameController::storeClick(Coordinate clickCoordinate){
	view.addPointOfInterest(coordToScreen(clickCoordinate));
	clickHistory.push_back(clickCoordinate);
}

Coordinate GameController::getLastClick(){
	return getPastClick(0);
}

Coordinate GameController::getPastClick(int howLongAgo){
	if (howLongAgo < clickHistory.size()){
		return clickHistory[clickHistory.size() - 1 - howLongAgo];
	}
	return Coordinate(-100,-100);
}

void GameController::clearClickHistory(){
	view.clearPointsOfInterest();
	clickHistory.clear();
}

bool GameController::hasClickHistory(){
	return !clickHistory.empty();
}


int GameController::getClickHistorySize(){
	return clickHistory.size();
}





/**
 * Handles a click that is actually on the tiles of the board. Either constructs a road or a settlement based on the control buttons the user has clicked.
 * @param screenCoord Where the user clicked on screen.
 * @return Whether this event was handled by this element. Always true.
 */
bool GameController::handleBoardEvent(ScreenCoordinate screenCoord) {
	auto coord = screenToCoord(screenCoord);

	switch (getState()){
	case BUILDROAD:
		std::cout << "BUILDROAD\n";
		if(!hasClickHistory()) {
			storeClick(coord);
		} else {
			if (model.PlaceRoad(getLastClick(), coord, *model.getPlayers()[0]));
			{
				popState();
			}
			clearClickHistory();
		}
		break;
	case ROBBER:
		std::cout << "ROBBER\n";
		model.moveRobber(coord);
		if (getState() == KNIGHT_DEVCARD)
			popState();
		break;
	case BUILDROAD_DEVCARD:
		storeClick(coord);
		if(getClickHistorySize() >= 4){
			using namespace std::placeholders;
			view.addElement(makeConfirmationDialogue(
				std::bind(&GameController::handleConfirmRoadCard, this, _1),
				std::bind(&GameController::handleCancelRoadCard, this, _1), {{.2, .3}, {.8, .6}}));
			pushState(MODALSTATE);
		}
		break;
	case KNIGHT_DEVCARD:
		//model.getCurrentPlayer().playKnight(coord, opponent);
		break;
	case YEAROFPLENTY_DEVCARD:
		//@ TODO Need to select a resource
		model.getCurrentPlayer().playYearOfPlenty(0);
	case MONOPOLY_DEVCARD:
		//@TODO Need to select a resource
		model.getCurrentPlayer().playMonopoly(0);
	case BUILDSETTLEMENT:
		std::cout << "BUILDSETTLEMENT\n";
		model.PlaceSettlement(coord, *model.getPlayers()[0]);
		popState();
		break;
	default:
		break;
	}
	return true;
}

bool GameController::handleCancelButtonEvent(ScreenCoordinate){
	while(getState() != BASESTATE){
		popState();
	}
	clearClickHistory();
	return true;
}


/**
 * Handles a click on the "create road" button. Changes the internal state to indicate the user is going to be making roads on the board.
 * @param coord The place the user clicked on screen
 * @return Whether this event was handled by this element. Always true.
 */
bool GameController::handleRoadButtonEvent(ScreenCoordinate coord) {
	clearClickHistory();
	if(getState() != BASESTATE){
		return true;
	}
	pushState(BUILDROAD);
	return true;
}

/**
 * Handles a click on the "create settlement" button. Changes the internal state to indicate the user is going to be making roads on the board.
 * @param coord The place the user clicked on screen.
 * @return Whether thi sevent was handled by this element. Always true.
 */
bool GameController::handleSettlementButtonEvent(ScreenCoordinate coord) {
	if(getState() != BASESTATE){
		return true;
	}
	pushState(BUILDSETTLEMENT);
	return true;
}

bool GameController::handleRoadCardButtonEvent(ScreenCoordinate coord){
	if(getState() != BASESTATE){
		return true;
	}
	clearClickHistory();
	pushState(BUILDROAD_DEVCARD);
	return true;
}

bool GameController::handleConfirmRoadCard(ScreenCoordinate coord){
	//play card
	std::cout << "CONFIRMED!";
	model.getCurrentPlayer().playRoadBuilding(getPastClick(3), getPastClick(2), getPastClick(1), getPastClick(0));
	view.removeLastElement();
	return handleCancelButtonEvent(coord);
}

bool GameController::handleCancelRoadCard(ScreenCoordinate coord){
	std::cout << "CANCELLED!";
	view.removeLastElement();
	return handleCancelButtonEvent(coord);
}


bool GameController::handleKnightCardButtonEvent(ScreenCoordinate){
	if(getState() != BASESTATE){
		return true;
	}
	pushState(KNIGHT_DEVCARD);
	pushState(ROBBER);
	return true;
}

bool GameController::handleYearOfPlentyCardButtonEvent(ScreenCoordinate){
	if(getState() != BASESTATE){
		return true;
	}
	pushState(YEAROFPLENTY_DEVCARD);
	return true;
}
bool GameController::handleMonopolyCardButtonEvent(ScreenCoordinate){
	std::cout <<"CONFIRMED!!!";
	return true;
}

bool GameController::handleVictoryPointCardButtonEvent(ScreenCoordinate){
	std::cout << "CANCELED!!!";
	return true;
}
















