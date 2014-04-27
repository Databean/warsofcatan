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
	
	view.addElement(makeViewButtonText(std::bind(&GameController::handleRoadButtonEvent, this, _1), {{0, 0}, {0.1, 0.10}}, font, fontSize, "Road |"));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleCityButtonEvent, this, _1), {{0.10, 0.0}, {0.20, 0.1}}, font, fontSize, "City |"));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleSettlementButtonEvent, this, _1), {{0.20, 0.0}, {0.33, 0.1}}, font, fontSize, "Settlement"));
	view.addElement(makeViewButtonText(std::bind(&GameController::nextTurn, this, _1), {{0, 0.3}, {0.1, 0.4}}, font, fontSize, "End Turn"));
	
	auto playerTopY = 0.82;
	for(auto i = 0; i < model.getNoOfPlayers(); i++) {
		auto width = 0.15;
		Player& player = model.getPlayer(i);
		view.addElement(makeViewButtonText(std::bind(&GameController::handlePlayerClick, this, _1, std::ref(player)), {{1.0 - width, playerTopY - 0.05}, {1.0, playerTopY}}, font, fontSize, player.getName()));
		playerTopY -= 0.05;
	}
	view.addElement(makeViewButtonText(std::bind(&GameController::handleBankClick, this, _1), {{0, 0.8}, {0.1, 0.9}}, font, fontSize, "Bank"));
	
	view.addElement(makeViewButtonText(std::bind(&GameController::handleCancelButtonEvent, this, _1), {{.92, .96}, {1.0, 1.0}}, font, fontSize, "Cancel"));
	
	view.addElement(makeViewButtonText(std::bind(&GameController::handleBuyDevelopmentCardButtonEvent, this, _1), {{.85, .23}, {1, .30}}, font, fontSize, "Development Cards"));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleRoadCardButtonEvent, this, _1), {{0.85, 0.0}, {0.97, 0.05}}, font, fontSize, "Road Building "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleKnightCardButtonEvent, this, _1), {{0.85, 0.05}, {0.97, 0.10}},  font, fontSize, "Knight "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleYearOfPlentyCardButtonEvent, this, _1), {{0.85, 0.10}, {0.97, 0.15}},  font, fontSize, "Year of Plenty "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleMonopolyCardButtonEvent, this, _1), {{0.85, 0.15}, {0.97, 0.20}},  font, fontSize, "Monopoly "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleVictoryPointCardButtonEvent, this, _1), {{0.85, 0.20}, {0.97, 0.25}},  font, fontSize, "Victory Point "));

	view.addElement(makeViewButtonText(std::bind(&GameController::handleWoodButtonEvent, this, _1), {{.85, .35}, {.97, .40}}, font, fontSize, "Wood "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleSheepButtonEvent, this, _1), {{.85, .40}, {.97, .45}}, font, fontSize, "Sheep "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleOreButtonEvent, this, _1), {{.85, .45}, {.97, .50}}, font, fontSize, "Ore "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleBrickButtonEvent, this, _1), {{.85, .50}, {.97, .55}}, font, fontSize, "Brick "));
	view.addElement(makeViewButtonText(std::bind(&GameController::handleWheatButtonEvent, this, _1), {{.85, .55}, {.97, .60}}, font, fontSize, "Wheat "));
	
	view.addElement(100, makeViewButton(std::bind(&GameController::handleBoardEvent, this, _1), {{0, 0}, {1, 1}}));
	
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

void printPlayerInfo(const Player& player) {
	auto color = player.getColor();
	std::cout << player.getName() << "'s turn. (" << std::get<0>(color) << ", " << std::get<1>(color) << ", " << std::get<2>(color) <<")" << std::endl;
	std::cout << "Wood: " << player.getWood() << ", Brick: " << player.getBrick() << ", Ore: " << player.getOre() << ", Wheat: " << player.getWheat() << ", Wool: " << player.getWool() << std::endl;
}

/**
 *  calls a function to advance turn, check for victory and roll dice
 */
bool GameController::nextTurn(ScreenCoordinate) {
	if(getState() != BASESTATE){
		return false;
	}

	model.endTurn();
	if (model.getDice().getFirst() + model.getDice().getSecond() == 7)
	{
		view.setControlStateText("The Robber is out! Click a tile to place it!");
		pushState(ROBBER);
	}

	printPlayerInfo(model.getCurrentPlayer());
	return true;
}



/**
 * Handles a click that is actually on the tiles of the board. Either constructs a road or a settlement based on the control buttons the user has clicked.
 * @param screenCoord Where the user clicked on screen.
 * @return Whether this event was handled by this element. Always true.
 */
bool GameController::handleBoardEvent(ScreenCoordinate screenCoord) {
	printPlayerInfo(model.getCurrentPlayer());
	auto coord = screenToCoord(screenCoord);
	std::vector<Settlement*> neighbors;
	
	switch (getState()){
	case BUILDROAD:
		if(!hasClickHistory()) {
			storeClick(coord);
		} else {
			if (model.buyRoad(getLastClick(), coord, model.getCurrentPlayer()));
			{
				handleCancelButtonEvent(screenCoord);
			}
			clearClickHistory();
		}
		break;
	case ROBBER:
		robPlayers();
		if(!hasClickHistory())
		{
			storeClick(coord);
			view.setControlStateText("Click the name of the player you want to rob(choose yourself to not rob anyone)");
		}

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
		if(!hasClickHistory()){
			storeClick(coord);
			view.setControlStateText("Select a player around that tile to steal from (Select yourself if you don't want to rob anyone)");
		}
		break;
	case BUILDSETTLEMENT:
		std::cout << "attempting to buy a settlement" << std::endl;
		model.buySettlement(coord, model.getCurrentPlayer());
		handleCancelButtonEvent(screenCoord);
		break;
	case BUILDCITY:
		std::cout << "attempting to build a city" << std::endl;
		model.buyUpgradeOnSettlement(coord, model.getCurrentPlayer());
		handleCancelButtonEvent(screenCoord);
		break;
	default:
		break;
	}
	return true;
}

void GameController :: robPlayers() {
	for (int i = 0; i < model.getNoOfPlayers(); i++) {
		int resources[5];
		model.getPlayer(i).checkResources(resources);
		//int rSum = resources[0] + resources[1] + resources[2] + resources[3] + resources[4];
		int rSum = model.getPlayer(i).getWood() + 
		model.getPlayer(i).getOre() + 
		model.getPlayer(i).getBrick() + 
		model.getPlayer(i).getWheat() + 
		model.getPlayer(i).getWool(); 

		if (rSum > 7) {
			for (int j = 0 ; j < rSum/2; j++) {
				model.getPlayer(i).addResource(model.getPlayer(i).getRandomResource(), -1);

			}
		}
	}
}

/**
 * Handles the event when the cancel button in the top right corner of the screen is pressed. This will
 * reset the control state back to the base state.
 */
bool GameController::handleCancelButtonEvent(ScreenCoordinate){
	if(getState() == ROBBER){
		clearClickHistory();
		return true;
	}

	while(getState() != BASESTATE){
		popState();
	}
	view.setControlStateText(" ");
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
		return false;
	}
	view.setControlStateText("Click two adjacent corner points to place a road. (1Br and 1Wd)");
	pushState(BUILDROAD);
	return true;
}

/**
 * Handles a click on the "create settlement" button. Changes the internal state to indicate the user is going to be making settlements on the board.
 * @param coord The place the user clicked on screen.
 * @return Whether this event was handled by this element. Always true.
 */
bool GameController::handleSettlementButtonEvent(ScreenCoordinate coord) {
	if(getState() != BASESTATE){
		return false;
	}
	view.setControlStateText("Click on a corner tile to build a settlement there. (1Br, 1Wh, 1Wd, and 1Sh)");
	pushState(BUILDSETTLEMENT);
	return true;
}

/**
 * Handles a click on the "create city" button. Changes the internal state to indicate the user is going to be upgrading settlements to cities on the board.
 * @param coord The place the user clicked on screen.
 * @return Whether this event was handled by this element. Always true.
 */
bool GameController::handleCityButtonEvent(ScreenCoordinate coord) {
	if(getState() != BASESTATE) {
		return false;
	}

	view.setControlStateText("Click on a settlement to upgrade it to a city. (2Wh and 3 Ore)");
	pushState(BUILDCITY);
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

	view.setControlStateText("Playing Road Building Card: Click on adjacent points for the roads you want.");
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

bool GameController::handleBuyDevelopmentCardButtonEvent(ScreenCoordinate coord){
	model.buyCard(model.getCurrentPlayer());
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

	view.setControlStateText("Select a tile that you want to place the robber on");
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

	view.setControlStateText("Playing Year of Plenty Card: Click on the resource you want on the side of the screen");
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
	view.setControlStateText("Playing Monopoly Card: Click on the resource you want on the side of the screen");
	pushState(MONOPOLY_DEVCARD);
	return true;
}

/**
 * Handles a click on the VictoryPoint card button. Currently does nothing because victory point cards don't do anything
 * @param coord The place the user clicked
 * @return true
 */
bool GameController::handleVictoryPointCardButtonEvent(ScreenCoordinate coord){
	if(getState() != BASESTATE){
		return false;
	}
	//pushState(VICTORYPOINT_DEVCARD);
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
	if(getState() == BASESTATE){
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

		view.addElement(priority, std::unique_ptr<ViewElement>(new TradingView(initiating.getName(), receiving.getName(), tradeFunction, cancelFunction, initial)));
		std::cout << player.getName() << std::endl;
		return true;

	//KNIGHT
	}else if(getState() == KNIGHT_DEVCARD){
		if(hasClickHistory()){
			model.getCurrentPlayer().playKnight(getLastClick(), player);
			return handleCancelButtonEvent(coord);
		}

	//ROBBER
	}else if(getState() == ROBBER){
		if(hasClickHistory() && (model.canRobberRob(player, getLastClick()) || player == model.getCurrentPlayer()) &&
				model.moveRobber(getPastClick(0))){

			int resourceToSteal = player.getRandomResource();
			if(resourceToSteal >= 0){
				model.getCurrentPlayer().addResource(resourceToSteal, 1);
				player.addResource(resourceToSteal, -1);
			}
			popState();
			return handleCancelButtonEvent(coord);
		}
	}
	
}

/**
 * Handle a player beginning a trade with the bank.
 */
bool GameController::handleBankClick(ScreenCoordinate screenCoord) {
	
	auto priority = -10;
	
	auto tradeFunction = [this, priority](std::array<int, 5> offer, ScreenCoordinate coord) {
		std::array<int, 5> splitOffer;
		std::array<int, 5> splitDemand;
		for(int i = 0; i < 5; i++) {
			std::cout << "offer " << i << " " << offer[i] << std::endl;
			splitOffer[i] = offer[i] > 0 ? 0 : -offer[i];
			splitDemand[i] = offer[i] < 0 ? 0 : offer[i];
		}
		if(model.getCurrentPlayer().makeBankTrade(splitOffer, splitDemand)) {
			view.removeElement(priority);
		}
		return true;
	};
	
	auto cancelFunction = [this, priority](ScreenCoordinate coord) {
		view.removeElement(priority);
		return true;
	};
	
	view.addElement(priority, std::unique_ptr<ViewElement>(new TradingView(model.getCurrentPlayer().getName(), "Bank", tradeFunction, cancelFunction, {0, 0, 0, 0, 0})));
	
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
		
		view.addElement(priority, std::unique_ptr<ViewElement>(new TradingView(initiating.getName(), receiving.getName(), tradeFunction, cancelFunction, counterOffer)));
	}
	return true;
}

/**
 * Handles clicks on the resource button for Wood
 */
bool GameController::handleWoodButtonEvent(ScreenCoordinate coord){
	if(getState() == YEAROFPLENTY_DEVCARD){
		model.getCurrentPlayer().playYearOfPlenty(WOOD);
		return handleCancelButtonEvent(coord);
	}else if(getState() == MONOPOLY_DEVCARD){
		model.getCurrentPlayer().playMonopoly(WOOD);
		return handleCancelButtonEvent(coord);
	}
	return false;
}

/**
 * Handles clicks on the resource button for Sheep
 */
bool GameController::handleSheepButtonEvent(ScreenCoordinate coord){
	if(getState() == YEAROFPLENTY_DEVCARD){
		model.getCurrentPlayer().playYearOfPlenty(SHEEP);
		return handleCancelButtonEvent(coord);
	}else if(getState() == MONOPOLY_DEVCARD){
		model.getCurrentPlayer().playMonopoly(SHEEP);
		return handleCancelButtonEvent(coord);
	}
	return false;

}

/**
 * Handles clicks on the resource button for Wheat
 */
bool GameController::handleWheatButtonEvent(ScreenCoordinate coord){
	if(getState() == YEAROFPLENTY_DEVCARD){
		model.getCurrentPlayer().playYearOfPlenty(WHEAT);
		return handleCancelButtonEvent(coord);
	}else if(getState() == MONOPOLY_DEVCARD){
		model.getCurrentPlayer().playMonopoly(WHEAT);
		return handleCancelButtonEvent(coord);
	}
	return false;

}

/**
 * Handles clicks on the resource button for Ore
 */
bool GameController::handleOreButtonEvent(ScreenCoordinate coord){
	if(getState() == YEAROFPLENTY_DEVCARD){
		model.getCurrentPlayer().playYearOfPlenty(STONE);
		return handleCancelButtonEvent(coord);
	}else if(getState() == MONOPOLY_DEVCARD){
		model.getCurrentPlayer().playMonopoly(STONE);
		return handleCancelButtonEvent(coord);
	}
	return false;

}

/**
 * Handles clicks on the resource button for Brick
 */
bool GameController::handleBrickButtonEvent(ScreenCoordinate coord){
	if(getState() == YEAROFPLENTY_DEVCARD){
		model.getCurrentPlayer().playYearOfPlenty(BRICK);
		return handleCancelButtonEvent(coord);
	}else if(getState() == MONOPOLY_DEVCARD){
		model.getCurrentPlayer().playMonopoly(BRICK);
		return handleCancelButtonEvent(coord);
	}
	return false;

}
