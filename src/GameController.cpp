#include "GameController.h"

#include <iostream>
#include <functional>
#include <memory>

#include "Config.h"
#include "GameBoard.h"
#include "GameView.h"
#include "Renderer.h"
#include "Player.h"

/**
 * Initialize the game controller. Adds the buttons for user control to the view, binding them to GameController methods.
 * @param model The GameBoard model that is drawn to the screen.
 * @param view The GameView that is used to draw the model.
 */
GameController::GameController(GameBoard& model, GameView& view) : model(model), view(view) {
	using namespace std::placeholders;


	auto font = getGraphicsConfig()["font.path"];
	auto fontSize = getGraphicsConfig()["font.size"];

	view.addElement(makeViewButton(std::bind(&GameController::handleBoardEvent, this, _1), {{0, 0}, {1, 1}}));

	view.addElement(makeViewButtonColor(std::bind(&GameController::nextTurn, this, _1), {{0, 0.2}, {0.1, 0.3}}, std::make_tuple(0.f, 0.f, 1.f)));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleRoadButtonEvent, this, _1), {{0, 0}, {0.1, 0.1}}, font, fontSize, "Build Road "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleSettlementButtonEvent, this, _1), {{0, 0.1}, {0.1, 0.2}}, font, fontSize, "Build Settlement "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleCityButtonEvent, this, _1), {{0, 0.2}, {0.2, 0.3}}, font, fontSize, "Build City "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleWonderButtonEvent, this, _1), {{0, 0.3}, {0.3, 0.4}}, font, fontSize, "Build Wonder "));
	
	auto playerTopY = 0.9;
	for(auto i = 0; i < model.getNoOfPlayers(); i++) {
		auto width = 0.15;
		Player& player = model.getPlayer(i);
		view.addElement(makeViewButtonText(std::bind(&GameController::handlePlayerClick, this, _1, std::ref(player)), {{1.0 - width, playerTopY - 0.05}, {1.0, playerTopY}}, font, fontSize, player.getName()));
		playerTopY -= 0.05;
	}
	
	view.addElement(makeViewButtonColor(std::bind(&GameController::handleCancelButtonEvent, this, _1), {{.95, .95}, {1.0, 1.0}}, std::make_tuple(1.f, 0.0f, 0.f)));
	
	view.addElement(makeViewButtonText(std::bind(&GameController::handleRoadCardButtonEvent, this, _1), {{0.85, 0.0}, {0.97, 0.05}}, font, fontSize, "Road Building "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleKnightCardButtonEvent, this, _1), {{0.85, 0.05}, {0.97, 0.10}},  font, fontSize, "Knight "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleYearOfPlentyCardButtonEvent, this, _1), {{0.85, 0.10}, {0.97, 0.15}},  font, fontSize, "Year of Plenty "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleMonopolyCardButtonEvent, this, _1), {{0.85, 0.15}, {0.97, 0.20}},  font, fontSize, "Monopoly "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleVictoryPointCardButtonEvent, this, _1), {{0.85, 0.20}, {0.97, 0.25}},  font, fontSize, "Victory Point "));



	stateStack.push_back(BASESTATE);
}

/**
 * Destroy the GameController.
 */
GameController::~GameController() {
	
}

/**
 * Pushes the given state onto the control stack
 */
void GameController::pushState(ControlState newState){
	if (newState != BASESTATE){
		stateStack.push_back(newState);
	}
}

/**
 * Pops the latest state from the control stack
 */
ControlState GameController::popState(){
	ControlState currState = getState();
	if(currState == BASESTATE){
		return currState;
	}
	stateStack.pop_back();
	return currState;
}

/**
 * returns the current state of the controller
 */
ControlState GameController::getState(){
	return stateStack.back();
}

/**
 * Stores a Coordinate in the clikc history
 */
void GameController::storeClick(Coordinate clickCoordinate){
	view.addPointOfInterest(coordToScreen(clickCoordinate));
	clickHistory.push_back(clickCoordinate);
}

/**
 * Returns the last click stored in the history
 */
Coordinate GameController::getLastClick(){
	return getPastClick(0);
}

/**
 * Gets a click from param clicks ago
 * @ param an integer
 */
Coordinate GameController::getPastClick(int howLongAgo){
	if (howLongAgo < clickHistory.size()){
		return clickHistory[clickHistory.size() - 1 - howLongAgo];
	}
	return Coordinate(-100,-100);
}

/**
 * Clears the history of clicks
 */
void GameController::clearClickHistory(){
	view.clearPointsOfInterest();
	clickHistory.clear();
}

/**
 * checks if there is a history of past clicks
 */
bool GameController::hasClickHistory(){
	return !clickHistory.empty();
}

/**
 * Returns the number of past clicks recorded
 */
int GameController::getClickHistorySize(){
	return clickHistory.size();
}





/**
 *  calls a function to advance turn, check for victory and roll dice
 */
bool GameController::nextTurn(ScreenCoordinate) {
	model.endTurn();
	return true;
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
		//model.moveRobber(coord);
		popState();
		break;
	case BUILDROAD_DEVCARD:
		storeClick(coord);
		if(getClickHistorySize() >= 4){
			using namespace std::placeholders;
			view.addElement(28, makeConfirmationDialogue(
				std::bind(&GameController::handleConfirmRoadCard, this, _1),
				std::bind(&GameController::handleCancelDialogueEvent, this, _1), {{.2, .3}, {.8, .6}},
				"Use road building card on these points?"));
			pushState(MODALSTATE);
		}
		break;
	case KNIGHT_DEVCARD:
		//model.getCurrentPlayer().playKnight(coord, opponent);
		popState();
		break;
	case YEAROFPLENTY_DEVCARD:
		model.getCurrentPlayer().playYearOfPlenty(model.getResourceTile(coord).getType());
		popState();
		break;
	case MONOPOLY_DEVCARD:
		model.getCurrentPlayer().playYearOfPlenty(model.getResourceTile(coord).getType());
		popState();
		break;
	case VICTORYPOINT_DEVCARD:
		model.getCurrentPlayer().playVictoryCard();
		popState();
		break;
	case BUILDSETTLEMENT:
		std::cout << "BUILDSETTLEMENT\n";
		model.PlaceSettlement(coord, *model.getPlayers()[0]);
		popState();
		break;
	case BUILDCITY:
		std::cout << "BUILDCITY\n";
		model.PlaceCity(coord, *model.getPlayers()[0]);
		popState();
		break;
	case BUILDWONDER:
		std::cout << "BUILDWONDER\n";
		model.PlaceWonder(coord, *model.getPlayers()[0]);
		popState();
		break;
	default:
		break;
	}
	return true;
}

/**
 * Handles the event when the cancel button in the top right corner of the screen is pressed. This will
 * reset the control state back to the base state.
 */
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
 * Handles a click on the "create settlement" button.
 * @param coord The place the user clicked on screen.
 * @return Whether this event was handled by this element. Always true.
 */
bool GameController::handleSettlementButtonEvent(ScreenCoordinate coord) {
	if(getState() != BASESTATE){
		return true;
	}
	pushState(BUILDSETTLEMENT);
	return true;
}

/**
 * Handles a click on the "create city" button.
 * @param coord The place the user clicked on screen.
 * @return Whether this event was handled by this element. Always true.
 */
bool GameController::handleCityButtonEvent(ScreenCoordinate coord) {
	if(getState() != BASESTATE){
		return true;
	}
	pushState(BUILDCITY);
	return true;
}

/**
 * Handles a click on the "create wonder" button.
 * @param coord The place the user clicked on screen.
 * @return Whether this event was handled by this element. Always true.
 */
bool GameController::handleWonderButtonEvent(ScreenCoordinate coord) {
	if(getState() != BASESTATE){
		return true;
	}
	pushState(BUILDWONDER);
	return true;
}


/**
 * Handles a click on the road Building Card button. This changes the control state to indicate the user is going to be building roads on the board.
 * @param coord The place the user clicked on the screen
 * @return whether this event was handles by this element. Always true.
 */
bool GameController::handleRoadCardButtonEvent(ScreenCoordinate coord){
	if(getState() != BASESTATE){
		return true;
	}
	clearClickHistory();
	pushState(BUILDROAD_DEVCARD);
	return true;
}

/**
 * Handles a click on the confirm button in the confirmation dialogues for the Road Building Card. This will attempt to place the roads the
 * user chose and clear the control state.
 * @param coord The place the user clicked on the screen
 * @return true
 */
bool GameController::handleConfirmRoadCard(ScreenCoordinate coord){
	model.getCurrentPlayer().playRoadBuilding(getPastClick(3), getPastClick(2), getPastClick(1), getPastClick(0));
	view.removeElement(28);
	return handleCancelButtonEvent(coord);
}

/**
 * Handles a click on the cancel button in the confrimation dialogue for the Road Building Card. This will clear the control state back to default.
 * @param coord The place the user clicked on the screen
 * @return true
 */
bool GameController::handleCancelDialogueEvent(ScreenCoordinate coord){
	view.removeElement(28);
	return handleCancelButtonEvent(coord);
}

/**
 * Handles a click on the Knight Card button.
 * @param coord The place the user clicked
 * @return true
 */
bool GameController::handleKnightCardButtonEvent(ScreenCoordinate coord){
	if(getState() != BASESTATE){
		return true;
	}
	pushState(KNIGHT_DEVCARD);
	return true;
}

/**
 * Handles a click on the Year of Plenty Card button
 * @param coord The place the user clicked
 * @return true
 */
bool GameController::handleYearOfPlentyCardButtonEvent(ScreenCoordinate coord){
	if(getState() != BASESTATE){
		return true;
	}
	pushState(YEAROFPLENTY_DEVCARD);
	return true;
}

/**
 * Handles a click on the Monopoly Card Button
 * @param coord The place the user clicked
 * @return true;
 */
bool GameController::handleMonopolyCardButtonEvent(ScreenCoordinate coord){
	if(getState() != BASESTATE){
		return true;
	}
	pushState(MONOPOLY_DEVCARD);
	return true;
}

/**
 * Handles a click on the VictoryPoint card button. Will push the victory point card state to the control stack
 * @param coord The place the user clicked
 * @return true
 */
bool GameController::handleVictoryPointCardButtonEvent(ScreenCoordinate coord){
	if(getState() != BASESTATE){
		return true;
	}
	pushState(VICTORYPOINT_DEVCARD);
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
		std::array<int, 5> splitOffer;
		std::array<int, 5> splitDemand;
		for(int i = 0; i < 5; i++) {
			splitOffer[i] = counterOffer[i] < 0 ? 0 : -counterOffer[i];
			splitDemand[i] = counterOffer[i] < 0 ? 0 : counterOffer[i];
		}
		initiating.acceptOffer(receiving, splitOffer, splitDemand);
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

