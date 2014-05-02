#include "GameView.h"

#define _USE_MATH_DEFINES

#include <stdexcept>
#include <cmath>

#include "Config.h"
#include "GameBoard.h"
#include "GameController.h"
#include "Renderer.h"
#include "City.h"
#include "Wonder.h"
#include "ViewElement.h"

using std::make_pair;
using std::pair;
using std::runtime_error;
using std::string;
using std::unique_ptr;

float DiceXCoords[3] = { 9.f, 134.f, 259.f };
float DiceYCoords[2] = { 3.f, 142.f };
std::pair<std::pair<float, float>, std::pair<float, float>> lDieScreenLoc =
		make_pair(make_pair(0.6f, 0.82f), make_pair(0.66f, 0.88f));
std::pair<std::pair<float, float>, std::pair<float, float>> rDieScreenLoc =
		make_pair(make_pair(0.68f, 0.82f), make_pair(0.74f, 0.88f));

//The length of a side of the dice on the sprite sheet in pixels.
#define DIE_SCREEN_SIDE_LENGTH 105.f

/**
 * Constrct a GameView.
 * @param model The GameBoard the view is displaying.
 */
GameView::GameView(GameBoard& model) :
		model(model) {
	controlStateText = "Welcome to Wars of Catan";
	showTotals = false;
}

/**
 * Destroy a GameView.
 */
GameView::~GameView() {

}

/**
 * Draws the amount of development cards the current player has. If resources are set to be hidden,
 * it will display '?' instead.
 * @param font the style of font to use
 * @param fontSize the resolution of the font used
 * @return void
 */
void GameView::drawCardCount(std::string font, int fontSize) {
	std::pair<std::pair<float, float>, std::pair<float, float>> roadBuildingRect =
			getGraphicsConfig()["screen.roadBuildingCount.area"];
	std::pair<std::pair<float, float>, std::pair<float, float>> knightRect =
			getGraphicsConfig()["screen.knightCount.area"];
	std::pair<std::pair<float, float>, std::pair<float, float>> yearOfPlentyRect =
			getGraphicsConfig()["screen.yearOfPlentyCount.area"];
	std::pair<std::pair<float, float>, std::pair<float, float>> monopolyRect =
			getGraphicsConfig()["screen.monopolyCount.area"];
	std::pair<std::pair<float, float>, std::pair<float, float>> victoryPointRect =
			getGraphicsConfig()["screen.victoryPointCount.area"];

	if (showTotals == false) {
		renderText(font, fontSize, roadBuildingRect.first,
				roadBuildingRect.second, "?");		//Road Building
		renderText(font, fontSize, knightRect.first, knightRect.second, "?");//Knight
		renderText(font, fontSize, yearOfPlentyRect.first,
				yearOfPlentyRect.second, "?");		//Year of Plenty
		renderText(font, fontSize, monopolyRect.first, monopolyRect.second,
				"?");				//Monopoly
		renderText(font, fontSize, victoryPointRect.first,
				victoryPointRect.second, "?");		//Victory Point
		return;
	}

	renderText(font, fontSize, roadBuildingRect.first, roadBuildingRect.second,
			toString(model.getCurrentPlayer().getRoadBuildingCards()));	//Road Building
	renderText(font, fontSize, knightRect.first, knightRect.second,
			toString(model.getCurrentPlayer().getKnightCards()));		//Knight
	renderText(font, fontSize, yearOfPlentyRect.first, yearOfPlentyRect.second,
			toString(model.getCurrentPlayer().getYearOfPlentyCards()));	//Year of Plenty
	renderText(font, fontSize, monopolyRect.first, monopolyRect.second,
			toString(model.getCurrentPlayer().getMonopolyCards()));	//Monopoly
	renderText(font, fontSize, victoryPointRect.first, victoryPointRect.second,
			toString(model.getCurrentPlayer().getVictoryCards()));//Victory Point
}

/**
 * Draws the count of resources the currentPlayer has. If resources are set to be hidden,
 * it will display '?' instead.
 * @param font the style of font to use
 * @param fontSize the resolution of the font used
 * @return void
 */
void GameView::drawResourceCount(std::string font, int fontSize) {

	std::pair<std::pair<float, float>, std::pair<float, float>> woodRect =
			getGraphicsConfig()["screen.woodResources.area"];
	std::pair<std::pair<float, float>, std::pair<float, float>> sheepRect =
			getGraphicsConfig()["screen.sheepResources.area"];
	std::pair<std::pair<float, float>, std::pair<float, float>> oreRect =
			getGraphicsConfig()["screen.oreResources.area"];
	std::pair<std::pair<float, float>, std::pair<float, float>> brickRect =
			getGraphicsConfig()["screen.brickResources.area"];
	std::pair<std::pair<float, float>, std::pair<float, float>> wheatRect =
			getGraphicsConfig()["screen.wheatResources.area"];

	if (showTotals == false) {
		renderText(font, fontSize, woodRect.first, woodRect.second, "?");//Wood
		renderText(font, fontSize, sheepRect.first, sheepRect.second, "?");	//Sheep
		renderText(font, fontSize, oreRect.first, oreRect.second, "?");	//Ore
		renderText(font, fontSize, brickRect.first, brickRect.second, "?");	//Brick
		renderText(font, fontSize, wheatRect.first, wheatRect.second, "?"); //Wheat
		return;
	}

	renderText(font, fontSize, woodRect.first, woodRect.second,
			toString(model.getCurrentPlayer().getWood()));		//Wood
	renderText(font, fontSize, sheepRect.first, sheepRect.second,
			toString(model.getCurrentPlayer().getWool()));		//Sheep
	renderText(font, fontSize, oreRect.first, oreRect.second,
			toString(model.getCurrentPlayer().getOre()));		//Ore
	renderText(font, fontSize, brickRect.first, brickRect.second,
			toString(model.getCurrentPlayer().getBrick()));		//Brick
	renderText(font, fontSize, wheatRect.first, wheatRect.second,
			toString(model.getCurrentPlayer().getWheat()));		//Wheat

}

/**
 * Display the GameBoard to the screen as well as additional ViewElements.
 */
void GameView::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawingGameVisitor visitor(*this);
	model.accept(visitor);

	//Render UI items
	for (auto it = viewElements.rbegin(); it != viewElements.rend(); it++) {
		it->second->render();
	}
	//Render clicked points
	for (auto& it : pointsOfInterest) {
		highlightPoint(it);
	}

	auto font = getGraphicsConfig()["font.path"];
	auto fontSize = getGraphicsConfig()["font.size"];

	glColor3d(1, 1, 1);
	renderText(font, fontSize, { .0, .9 }, { .85, 1 }, controlStateText);

	renderText(font, fontSize, { .78, .82 }, { .8, .92 }, ">");
	renderText(font, fontSize, { .8, .82 }, { 1., .92 },
			model.getCurrentPlayer().getName());

	drawCardCount(font, fontSize);
	drawResourceCount(font, fontSize);

	if (model.hasWinner())
		renderText(font, fontSize, { .2, .4 }, { .8, .6 },
				model.getWinner().getName() + " Wins");

	glFlush();
}

/**
 * Pushes a coordinate into the points of interest history to be highlighted on screen
 * @param coord the point of interest to add
 */
void GameView::addPointOfInterest(ScreenCoordinate coord) {
	pointsOfInterest.push_back(coord);
}

/**
 * Clears the points of interest history, so those points will no longer be highlighted.
 */
void GameView::clearPointsOfInterest() {
	pointsOfInterest.clear();
}

/**
 * Highlights a coordinate with a small blue square
 * @param coord the coordinate to be highlighted on screen
 */
void GameView::highlightPoint(ScreenCoordinate & coord) {
	std::tuple<float, float, float> color(0.f, 0.f, 1.f);
	renderRectangle( { coord.first - .01, coord.second - .01 },
			{ coord.first + .01, coord.second + .01 }, color);
}

/**
 * Sets the text to be displayed at the top of the screen
 * @param newText the new text to be displayed on screen. An empty string will cause a crash.
 */
void GameView::setControlStateText(std::string newText) {
	controlStateText = newText;
}

/**
 * Handle raw input from SDL and convert into input the controller is familiar with.
 */
bool GameView::acceptInput(SDL_Event& event) {
	if (event.type == SDL_QUIT) {
		return false;
	} else if (event.type == SDL_MOUSEBUTTONUP) {
		ScreenCoordinate screen =
				{ (float) event.button.x / getGraphicsConfig()["screen.width"],
						1.f
								- (float) event.button.y
										/ getGraphicsConfig()["screen.height"] };
		for (auto& it : viewElements) {
			if (it.second->handleClick(screen)) {
				break;
			}
		}
	}
	return true;
}

/**
 * Add a ViewElement to be rendered and accept input. Defaults to the lowest priority.
 * @param element An owning pointer to the element to be added.
 */
void GameView::addElement(std::unique_ptr<ViewElement> element) {
	int newPriority = 0;
	if (viewElements.size() > 0) {
		newPriority = viewElements.rbegin()->first + 1;
	}
	addElement(newPriority, std::move(element));
}

/**
 * Add a ViewElement with a specific priority. This may override an existing element.
 */
void GameView::addElement(int newPriority,
		std::unique_ptr<ViewElement> element) {
	viewElements[newPriority] = std::move(element);
}

/**
 * Remove a ViewElement from the list, and return its owning pointer.
 * @param element The element to remove.
 * @return An owning pointer to the element that was removed.
 */
unique_ptr<ViewElement> GameView::removeElement(const ViewElement* element) {
	for (auto it = viewElements.begin(); it != viewElements.end(); it++) {
		if (it->second.get() == element) {
			auto ret = std::move(it->second);
			viewElements.erase(it);
			return ret;
		}
	}
	return unique_ptr<ViewElement>();
}

/**
 * Remove a ViewElement from the list, and return its owning pointer.
 * @param element The element to remove.
 * @return An owning pointer to the element that was removed.
 */
unique_ptr<ViewElement> GameView::removeElement(const ViewElement& element) {
	return removeElement(&element);
}

/**
 * Remove a ViewElement from the list, and return its owning pointer.
 * @param priority The priority of the element to remove.
 * @return An owning pointer to the element that was removed.
 */
unique_ptr<ViewElement> GameView::removeElement(int priority) {
	auto it = viewElements.find(priority);
	if (it == viewElements.end()) {
		return std::unique_ptr<ViewElement>();
	} else {
		auto ret = std::move(it->second);
		viewElements.erase(it);
		return ret;
	}
}

/**
 * Construct a DrawingGameVisitor with the view that it is drawing to.
 * @param view The view to draw to.
 */
DrawingGameVisitor::DrawingGameVisitor(GameView& view) :
		view(view) {

}

/**
 * Destroy the DrawingGameVisitor.
 */
DrawingGameVisitor::~DrawingGameVisitor() {

}

/**
 * Visit the game board.
 * @param model The GameBoard to be drawn.
 */
void DrawingGameVisitor::visit(GameBoard& model) {

}

/**
 * Draw a road.
 * @param road The road to draw.
 */
void DrawingGameVisitor::visit(Road& road) {
	static const auto roadWidth = 0.01;

	auto startScreenPos = coordToScreen(road.getStart());
	auto endScreenPos = coordToScreen(road.getEnd());

	auto roadAngle = 0.;
	/*
	 * Fixes a bug in road rendering of horizontal road. My best guess is that
	 * this is necessary because small floating point errors in the conversion from game coordinates to screen coordinates
	 * mess up the angle calculation.
	 */
	if (road.getEnd().second == road.getStart().second) {
		roadAngle = 0;
	} else {
		roadAngle = std::atan2(endScreenPos.first - startScreenPos.first,
				endScreenPos.second - startScreenPos.second);\
	}
	auto roadPerpAngle = roadAngle + (M_PI / 2.);

	auto cosPerp = std::cos(roadPerpAngle);
	auto sinPerp = std::sin(roadPerpAngle);

	auto color = road.getOwner().getColor();
	glColor3f(std::get<0>(color), std::get<1>(color), std::get<2>(color));

	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
	glVertex2d(startScreenPos.first - cosPerp * roadWidth,
			startScreenPos.second - sinPerp * roadWidth);
	glVertex2d(startScreenPos.first + cosPerp * roadWidth,
			startScreenPos.second + sinPerp * roadWidth);
	glVertex2d(endScreenPos.first + cosPerp * roadWidth,
			endScreenPos.second + sinPerp * roadWidth);
	glVertex2d(endScreenPos.first - cosPerp * roadWidth,
			endScreenPos.second - sinPerp * roadWidth);
	glEnd();
}

/**
 * Draw a settlement. Right now is just a diamond.
 * @param settlement The settlement to draw.
 */
void DrawingGameVisitor::visit(Settlement& settlement) {
	static const auto settlementRadius = 0.02;

	auto centerScreenPos = coordToScreen(settlement.getLocation());
	auto color = settlement.getOwner().getColor();

	renderRectangle(
			{ centerScreenPos.first + settlementRadius, centerScreenPos.second
					+ settlementRadius },
			{ centerScreenPos.first - settlementRadius, centerScreenPos.second
					- settlementRadius }, color);
}

/**
 * Draw a city. Right now is just a square.
 * @param city The city to draw.
 */
void DrawingGameVisitor::visit(City& city) {
	static const auto cityRadius = 0.03;

	auto centerScreenPos = coordToScreen(city.getLocation());
	auto color = city.getOwner().getColor();

	renderRectangle(
			{ centerScreenPos.first + cityRadius, centerScreenPos.second
					+ cityRadius },
			{ centerScreenPos.first - cityRadius, centerScreenPos.second
					- cityRadius }, color);
}

/**
 * Draw a wonder. Right now is just a square.
 * @param wonder The city to draw.
 */
void DrawingGameVisitor::visit(Wonder& wonder) {
	static const auto wonderRadius = 0.06;

	auto centerScreenPos = coordToScreen(wonder.getLocation());
	auto color = wonder.getOwner().getColor();

	renderRectangle(
			{ centerScreenPos.first + wonderRadius, centerScreenPos.second
					+ wonderRadius },
			{ centerScreenPos.first - wonderRadius, centerScreenPos.second
					- wonderRadius }, color);
}

/**
 * Draw a player.
 * @param player The player to draw.
 */
void DrawingGameVisitor::visit(Player& player) {

}

/**
 * Draw both dice.
 * @param dice the dice data structure for the board
 */
void DrawingGameVisitor::visit(GameDice& dice) {

	static const GLuint diceTextures = loadImageAsTexture(
			"resources/catan_dice_new.bmp");

	glColor3d(1.0, 1.0, 1.0);
	static std::map<int, std::pair<float, float>> topLeftOffset;
	//construct offset map

	for (int i = 1; i < 7; i++) {
		topLeftOffset.insert(
				make_pair(i,
						make_pair(DiceXCoords[(i - 1) % 3],
								DiceYCoords[i / 4])));
	}

	//draw Left die
	std::pair<float, float> topLeft =
			topLeftOffset.find(dice.getFirst())->second;
	std::pair<float, float> texture_bottomLeft = make_pair(topLeft.first,
			topLeft.second + DIE_SCREEN_SIDE_LENGTH);
	std::pair<float, float> texture_topRight = make_pair(
			topLeft.first + DIE_SCREEN_SIDE_LENGTH, topLeft.second);
	renderTexturedRectangle(lDieScreenLoc.first, lDieScreenLoc.second,
			texture_bottomLeft, texture_topRight, diceTextures);

	//draw Right die
	topLeft = topLeftOffset.find(dice.getSecond())->second;
	texture_bottomLeft = make_pair(topLeft.first,
			topLeft.second + DIE_SCREEN_SIDE_LENGTH);
	texture_topRight = make_pair(topLeft.first + DIE_SCREEN_SIDE_LENGTH,
			topLeft.second);
	renderTexturedRectangle(rDieScreenLoc.first, rDieScreenLoc.second,
			texture_bottomLeft, texture_topRight, diceTextures);
}

/**
 * Draw a resource tile.
 * @param tile The tile to draw.
 */
void DrawingGameVisitor::visit(ResourceTile& tile) {
	Coordinate coord = tile.getLocation();
	static const GLuint tileTextures = loadImageAsTexture(
			"resources/catan_sprite_sheet.bmp");

	glBindTexture(GL_TEXTURE_2D, tileTextures);

	//Map the locations of the top-right locations of the images in the sprite sheet
	//For resource tiles
	static const std::map<resourceType, pair<float, float>> topRightPoints = {
			make_pair(WOOD, make_pair(260.f, 17.f)), make_pair(BRICK,
					make_pair(629.f, 17.f)), make_pair(STONE,
					make_pair(262.f, 282.f)), make_pair(SHEEP,
					make_pair(621.f, 286.f)), make_pair(WHEAT,
					make_pair(267.f, 554.f)), make_pair(DESERT,
					make_pair(620.f, 555.f)) };
	static const std::vector<pair<float, float>> resourceTexOffsets = {
			make_pair(0.f, 0.f), make_pair(87.f, 130.f), make_pair(0.f, 261.f),
			make_pair(-174.f, 261.f), make_pair(-260.f, 130.f), make_pair(-175,
					-1) };

	//For number tiles
	static const std::map<int, pair<float, float>> numberTexPoints = {
			make_pair(2, make_pair(1238.5f, 70.5f)), make_pair(3,
					make_pair(1365.5f, 70.5f)), make_pair(4,
					make_pair(1238.5f, 178.5f)), make_pair(5,
					make_pair(1365.5f, 178.5f)), make_pair(6,
					make_pair(1238.5f, 286.5f)), make_pair(8,
					make_pair(1365.5f, 286.5f)), make_pair(9,
					make_pair(1238.5f, 404.5f)), make_pair(10,
					make_pair(1365.5f, 404.5f)), make_pair(11,
					make_pair(1238.5f, 515.5f)), make_pair(12,
					make_pair(1365.5f, 515.5f)) };
	static const float radius = 59.5f;
	static Coordinate adjacentCoordDiffs[] = { Coordinate(0, 1), Coordinate(1,
			0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0),
			Coordinate(-1, 1) };
	auto topRightPoint = topRightPoints.find(tile.getType());
	if (topRightPoint == topRightPoints.end()) {
		throw runtime_error("Cannot draw this tile; it is invalid.");
	}
	glColor3d(1.0, 1.0, 1.0);

	//Draw the resource tiles of the board
	glBegin(GL_TRIANGLE_FAN);
	auto average = averagePoint(resourceTexOffsets);
	texCoordPair(
			{ average.first + topRightPoint->second.first, average.second
					+ topRightPoint->second.second });
	vertexPair(coord);
	for (unsigned int i = 0; i < resourceTexOffsets.size(); i++) {
		texCoordPair(
				std::make_pair(
						resourceTexOffsets[i].first
								+ topRightPoint->second.first,
						resourceTexOffsets[i].second
								+ topRightPoint->second.second));
		Coordinate diff = adjacentCoordDiffs[i];
		vertexPair(
				Coordinate(coord.first + diff.first,
						coord.second + diff.second));
	}
	texCoordPair(topRightPoint->second);
	vertexPair(
			Coordinate(coord.first + adjacentCoordDiffs[0].first,
					coord.second + adjacentCoordDiffs[0].second));
	glEnd();
	//end drawing tiles of board

	//Draw the number tiles
	if (tile.getDiceValue() != 0
			|| tile.getBoard().getResourceTile(tile.getBoard().getRobber()).getType()
					== DESERT) {
		if (tile.getBoard().getRobber() == coord) { //draw the robber on this tile

			renderTexturedCircle(make_pair(1240.f, 643.f), 59.5f,
					coordToScreen(coord), 0.04, tileTextures);
		} else
			renderTexturedCircle(
					numberTexPoints.find(tile.getDiceValue())->second, radius,
					coordToScreen(coord), 0.04, tileTextures);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

}

/**
 * Draw a development card.
 * @param card The development card to draw.
 */
void DrawingGameVisitor::visit(DevelopmentCard& card) {

}

