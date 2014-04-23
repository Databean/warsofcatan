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

using std::make_pair;
using std::pair;
using std::runtime_error;
using std::string;
using std::unique_ptr;

float DiceXCoords[3] = {9.f, 134.f, 259.f};
float DiceYCoords[2] = {3.f, 142.f};
std::pair<float, float> lDieScreenLoc= make_pair(0.6f, 0.82f);
std::pair<float, float> rDieScreenLoc= make_pair(0.68f, 0.82f);

#define DIE_SIDE_LENGTH 0.06f
#define DIE_SCREEN_SIDE_LENGTH 105.f

#define EMPLACE_SQUARE_VERTEX(imXOff, imYOff, scXOff, scYOff) \
texCoordPair({texTopLeft.first + imXOff, texTopLeft.second + imYOff}); \
glVertex2d(screenTopLeft.first + scXOff, screenTopLeft.second + scYOff);


/**
 * Construct a ViewElement covering a particular rectangle on screen.
 * @param rect The rectangle on screen that the view element occupies.
 */
ViewElement::ViewElement(decltype(rect) rect) {
	using std::min;
	using std::max;
	
	//rect.first is the min x/min y corner
	//rect.second is the max x/max y corner
	this->rect.first = {min(rect.first.first, rect.second.first), min(rect.first.second, rect.second.second)};
	this->rect.second = {max(rect.first.first, rect.second.first), max(rect.first.second, rect.second.second)};
}

/**
 * Destroy the view element.
 */
ViewElement::~ViewElement() {
	
}

/**
 * The rectangle that the ViewElement occupies on screen.
 * @return The rectangle.
 */
decltype(ViewElement::rect) ViewElement::getRect() const {
	return rect;
}

/**
 * Determines if a ScreenCoordinate clicked on lies inside this ViewElement.
 * @return Whether the point is contained.
 */
bool ViewElement::containsPoint(ScreenCoordinate coord) const {
	return rect.first.first < coord.first &&
		rect.first.second < coord.second &&
		coord.first < rect.second.first &&
		coord.second < rect.second.second;
}

/**
 * Use this ViewElement's click handler if the coordinate clicked on lies inside the rectangle.
 */
bool ViewElement::handleClick(ScreenCoordinate coord) {
	if(containsPoint(coord)) {
		return clicked(coord);
	}
	return false;
}


/**
 * Constrct a GameView.
 * @param model The GameBoard the view is displaying.
 */
GameView::GameView(GameBoard& model) : model(model) {
	
}

/**
 * Destroy a GameView.
 */
GameView::~GameView() {
	
}


/**
 * Draws the amount of development cards the current player has.
 * @param font the style of font to use, fontSize the resolution of the font used
 * @return void
 */
void GameView::drawCardCount(std::string font, int fontSize){

	renderText(font, fontSize, {.85, .23}, {1, .30}, "Development Cards");

	renderText(font, fontSize, {0.97, 0.0}, {1.0, 0.05},
			toString(model.getCurrentPlayer().getRoadBuildingCards()));	//Road Building
	renderText(font, fontSize, {0.97, 0.05}, {1.0, 0.1},
			toString(model.getCurrentPlayer().getKnightCards()));		//Knight
	renderText(font, fontSize, {0.97, 0.1}, {1.0, 0.15},
			toString(model.getCurrentPlayer().getYearOfPlentyCards()));	//Year of Plenty
	renderText(font, fontSize, {0.97, 0.15}, {1.0, 0.2},
			toString(model.getCurrentPlayer().getMonopolyCards()));		//Monopoly
	renderText(font, fontSize, {0.97, 0.2}, {1.0, 0.25},
			toString(model.getCurrentPlayer().getVictoryCards()));		//Victory Point
}


/**
 * Display the GameBoard to the screen as well as additional ViewElements.
 */
void GameView::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DrawingGameVisitor visitor(*this);
	model.accept(visitor);
	
	for(auto it = viewElements.rbegin(); it != viewElements.rend(); it++) {
		it->second->render();
	}
	for(auto& it : pointsOfInterest) {
		highlightPoint(it);
	}
	
	auto font = getGraphicsConfig()["font.path"];
	auto fontSize = getGraphicsConfig()["font.size"];
	
	glColor3d(1, 1, 1);
	renderText(font, fontSize, {.2, .9}, {.8, 1}, "Settlers of Catan");
	
	drawCardCount(font, fontSize);

	glFlush();
}

/**
 * Handle raw input from SDL and convert into input the controller is familiar with.
 */
bool GameView::acceptInput(SDL_Event& event) {
	if(event.type == SDL_QUIT) {
		return false;
	} else if(event.type == SDL_MOUSEBUTTONUP) {
		ScreenCoordinate screen = {(float) event.button.x / 900.f, 1.f - (float) event.button.y / 800.f};
		for(auto& it : viewElements) {
			if(it.second->handleClick(screen)) {
				//break;
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
	if(viewElements.size() > 0) {
		newPriority = viewElements.rbegin()->first + 1;
	}
	addElement(newPriority, std::move(element));
}

/**
 * Add a ViewElement with a specific priority. This may override an existing element.
 */
void GameView::addElement(int newPriority, std::unique_ptr<ViewElement> element) {
	viewElements[newPriority] = std::move(element);
}

/**
 * Remove a ViewElement from the list, and return its owning pointer.
 * @param element The element to remove.
 * @return An owning pointer to the element that was removed.
 */
unique_ptr<ViewElement> GameView::removeElement(const ViewElement* element) {
	for(auto it = viewElements.begin(); it != viewElements.end(); it++) {
		if(it->second.get() == element) {
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
	if(it == viewElements.end()) {
		return std::unique_ptr<ViewElement>();
	} else {
		auto ret = std::move(it->second);
		viewElements.erase(it);
		return ret;
	}
}

/**
 * Construct a ViewButton.
 * @param action The action to take when the button is pressed.
 * @param rect The area on the screen that the button receives clicks from.
 */
ViewButton::ViewButton(std::function<bool(ScreenCoordinate)> action, std::pair<ScreenCoordinate, ScreenCoordinate> rect) : ViewElement(rect), action(action) {
	
}

/**
 * Destroy the ViewButton.
 */
ViewButton::~ViewButton() {
	
}

/**
 * Render the ViewButton.
 */
void ViewButton::render() {
	
}

/**
 * Process a click that happens on this ViewButton.
 * @param coord The coordinate clicked on.
 * @return If the ViewButton handled the event.
 */
bool ViewButton::clicked(ScreenCoordinate coord) {
	return action(coord);
}

/**
 * Construct the ViewButtonColor.
 * @param action The action to take when the button is clicked.
 * @param rect The rectangle the button receives clicks from and is drawn in.
 * @param color The color to draw the button.
 */
ViewButtonColor::ViewButtonColor(std::function<bool(ScreenCoordinate)> action, std::pair<ScreenCoordinate, ScreenCoordinate> rect, std::tuple<float, float, float> color) : ViewButton(action, rect), color(color) {
	
}

/**
 * Destroy the ViewButtonColor.
 */
ViewButtonColor::~ViewButtonColor() {
	
}

/**
 * Render the ViewButtonColor as a solid rectangle in the rect given.
 */
void ViewButtonColor::render() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(std::get<0>(color), std::get<1>(color), std::get<2>(color));
	auto topLeft = ViewElement::getRect().first;
	auto bottomRight = ViewElement::getRect().second;
	glBegin(GL_QUADS);
	glVertex2f(topLeft.first, topLeft.second);
	glVertex2f(bottomRight.first, topLeft.second);
	glVertex2f(bottomRight.first, bottomRight.second);
	glVertex2f(topLeft.first, bottomRight.second);
	glEnd();
}

/**
 * Construct a ViewButtonText.
 * @param action The callback to invoke on clicking the button.
 * @param rect The rectangle to draw it to the screen to and accept inputs from.
 * @param font The path to the font of the text to render.
 * @param fontSize The font size to draw the text at.
 * @param text The text to show.
 */
ViewButtonText::ViewButtonText(std::function<bool(ScreenCoordinate)> action, std::pair<ScreenCoordinate, ScreenCoordinate> rect, const std::string& font, int fontSize, const std::string& text)
	: ViewButton(action, rect), texture(0) {
	
	setText(font, fontSize, text);
}

/**
 * Destroy the ViewButtonText.
 */
ViewButtonText::~ViewButtonText() {
	glDeleteTextures(1, &texture);
}

/**
 * Change the text displayed by the ViewButtonText.
 * @param font The path to the font to draw the text in.
 * @param fontSize The size of the font to draw the text as.
 * @param text The text to draw.
 */
void ViewButtonText::setText(const std::string& font, int fontSize, const std::string& text) {
	if(texture != 0) {
		glDeleteTextures(1, &texture);
	}
	texture = loadTextAsTexture(font, fontSize, text);
}

/**
 * Render the ViewButtonText with the given text.
 */
void ViewButtonText::render() {
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0, 1.0, 1.0);
	auto topLeft = ViewElement::getRect().first;
	auto bottomRight = ViewElement::getRect().second;
	glBegin(GL_QUADS);
	glTexCoord2i(0, 1);
	glVertex2f(topLeft.first, topLeft.second);
	glTexCoord2i(1, 1);
	glVertex2f(bottomRight.first, topLeft.second);
	glTexCoord2i(1, 0);
	glVertex2f(bottomRight.first, bottomRight.second);
	glTexCoord2i(0, 0);
	glVertex2f(topLeft.first, bottomRight.second);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GameView::addPointOfInterest(ScreenCoordinate coord){
	pointsOfInterest.push_back(coord);
}

void GameView::clearPointsOfInterest(){
	pointsOfInterest.clear();
}

void GameView::highlightPoint(ScreenCoordinate & coord){
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(0., 0., 1.);
	glBegin(GL_QUADS);
	glVertex2f(coord.first - .01, coord.second - .01);
	glVertex2f(coord.first + .01 , coord.second - .01);
	glVertex2f(coord.first + .01, coord.second + .01);
	glVertex2f(coord.first - .01, coord.second + .01);
	glEnd();

}


/**
 * Construct a DrawingGameVisitor with the view that it is drawing to.
 * @param view The view to draw to.
 */
DrawingGameVisitor::DrawingGameVisitor(GameView& view) : view(view) {
	
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
	if(road.getEnd().second == road.getStart().second) {
		roadAngle = 0;
	} else {
		roadAngle = std::atan2(endScreenPos.first - startScreenPos.first, endScreenPos.second - startScreenPos.second);\
	}
	auto roadPerpAngle = roadAngle + (M_PI / 2.);
	
	auto cosPerp = std::cos(roadPerpAngle);
	auto sinPerp = std::sin(roadPerpAngle);
	
	auto color = road.getOwner().getColor();
	glColor3f(std::get<0>(color), std::get<1>(color), std::get<2>(color));

	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
	glVertex2d(startScreenPos.first - cosPerp * roadWidth, startScreenPos.second - sinPerp * roadWidth);
	glVertex2d(startScreenPos.first + cosPerp * roadWidth, startScreenPos.second + sinPerp * roadWidth);
	glVertex2d(endScreenPos.first + cosPerp * roadWidth, endScreenPos.second + sinPerp * roadWidth);
	glVertex2d(endScreenPos.first - cosPerp * roadWidth, endScreenPos.second - sinPerp * roadWidth);
	glEnd();
}

/**
 * Draw a settlement. Right now is just a diamond.
 * @param settlement The settlement to draw.
 */
void DrawingGameVisitor::visit(Settlement& settlement) {
	static const auto settlementRadius = 0.03;
	
	auto centerScreenPos = coordToScreen(settlement.getLocation());
	
	glBindTexture(GL_TEXTURE_2D, 0);
	auto color = settlement.getOwner().getColor();
	glColor3f(std::get<0>(color), std::get<1>(color), std::get<2>(color));

	glBegin(GL_QUADS);
	glVertex2d(centerScreenPos.first, centerScreenPos.second + settlementRadius);
	glVertex2d(centerScreenPos.first + settlementRadius, centerScreenPos.second);
	glVertex2d(centerScreenPos.first, centerScreenPos.second - settlementRadius);
	glVertex2d(centerScreenPos.first - settlementRadius, centerScreenPos.second);
	glEnd();
}

/**
 * Draw a city. Right now is just a square.
 * @param city The city to draw.
 */
void DrawingGameVisitor::visit(City& city) {
	static const auto cityRadius = 0.03;
	
	auto centerScreenPos = coordToScreen(city.getLocation());
	
	glBindTexture(GL_TEXTURE_2D, 0);

	auto color = city.getOwner().getColor();
	glColor3f(std::get<0>(color), std::get<1>(color), std::get<2>(color));

	glBegin(GL_QUADS);
	glVertex2d(centerScreenPos.first + cityRadius, centerScreenPos.second + cityRadius);
	glVertex2d(centerScreenPos.first + cityRadius, centerScreenPos.second - cityRadius);
	glVertex2d(centerScreenPos.first - cityRadius, centerScreenPos.second - cityRadius);
	glVertex2d(centerScreenPos.first - cityRadius, centerScreenPos.second + cityRadius);
	glEnd();
}

/**
 * Draw a wonder. Right now is just a square.
 * @param wonder The city to draw.
 */
void DrawingGameVisitor::visit(Wonder& wonder) {
	static const auto wonderRadius = 0.06;

	auto centerScreenPos = coordToScreen(wonder.getLocation());

	glBindTexture(GL_TEXTURE_2D, 0);

	auto color = wonder.getOwner().getColor();
	glColor3f(std::get<0>(color), std::get<1>(color), std::get<2>(color));

	glBegin(GL_QUADS);
	glVertex2d(centerScreenPos.first + wonderRadius, centerScreenPos.second + wonderRadius);
	glVertex2d(centerScreenPos.first + wonderRadius, centerScreenPos.second - wonderRadius);
	glVertex2d(centerScreenPos.first - wonderRadius, centerScreenPos.second - wonderRadius);
	glVertex2d(centerScreenPos.first - wonderRadius, centerScreenPos.second + wonderRadius);
	glEnd();
}

/**
 * Draw a player.
 * @param player The player to draw.
 */
void DrawingGameVisitor::visit(Player& player) {
	//TODO: draw cards, resources, etc.
}

/**
 * Convenience method to draw a circle that corresponds to a circular texture in the texture file.
 * @param texCenter The center of the texture.
 * @param texRadius The radius of the texture.
 * @param screenCenter The center of the circle drawn on screen.
 * @param screenRadius The radius of the circle drawn on screen.
 * @param articulation The number of points to draw in the circle.
 */
void drawTexturedCircle(std::pair<float, float> texCenter, float texRadius, std::pair<float, float> screenCenter, float screenRadius, int articulation = 20) {
	glBegin(GL_TRIANGLE_FAN);
	texCoordPair(texCenter);
	glVertex2f(screenCenter.first, screenCenter.second);
	for(int i = 0; i < articulation + 1; i++) {
		double angle = ((double) i) * (2. * M_PI) / (double)articulation;
		double tangle = ((double) -i) * (2. * M_PI) / (double)articulation;
		texCoordPair({texCenter.first + texRadius * std::cos(tangle), texCenter.second + texRadius * std::sin(tangle)});
		glVertex2d(screenCenter.first + (screenRadius * std::cos(angle)), screenCenter.second + (screenRadius * std::sin(angle)));
		
	}
	glEnd();
}

/**
 * Draw a textured square oriented parallel to the ground
 * @param texTopLeft image coordinates of the top left side of our square texture
 * @param sideLength image domain side length (in pixels)
 * @param screenTopLeft GL coordinates for the top left of our square to render
 * @param screenSideLength GL image domain square side length
 */
void drawTexturedRectangle(std::pair<float, float> texTopLeft, float sideLength, std::pair<float, float> screenTopLeft, float screenSideLength) {
	
	glBegin(GL_QUADS);

	EMPLACE_SQUARE_VERTEX(0.0f,			0.0f, 		0.0f,				0.0f)
	EMPLACE_SQUARE_VERTEX(sideLength,	0.0f, 		screenSideLength,	0.0f)
	EMPLACE_SQUARE_VERTEX(sideLength,	sideLength,	screenSideLength,	screenSideLength)
	EMPLACE_SQUARE_VERTEX(0.0f,			sideLength,	0.0f,				screenSideLength)

	glEnd();

}

/**
 * Draw both dice.
 * @param dice the dice data structure for the board
 */
void DrawingGameVisitor::visit(GameDice& dice) {

	static const GLuint diceTextures = loadImageAsTexture("resources/catan_dice_new.bmp");
	glBindTexture(GL_TEXTURE_2D, diceTextures);

	glColor3d(1.0, 1.0, 1.0);	
	static std::map<int, std::pair<float, float>> topLeftOffset;
	//construct offset map
	for (int i = 1; i < 7; i++) {
		
		//topLeftOffset.emplace(i, make_pair(DiceXCoords[(i-1)%3], DiceYCoords[i/4]));
		topLeftOffset.insert(make_pair(i, make_pair(DiceXCoords[(i-1)%3], DiceYCoords[i/4])));
	}
	

	drawTexturedRectangle(topLeftOffset.find(dice.getFirst())->second, DIE_SCREEN_SIDE_LENGTH, 
		lDieScreenLoc, DIE_SIDE_LENGTH);
		

	drawTexturedRectangle(topLeftOffset.find(dice.getSecond())->second, DIE_SCREEN_SIDE_LENGTH, 
		rDieScreenLoc, DIE_SIDE_LENGTH);


	glBindTexture(GL_TEXTURE_2D, 0);
	
}

/**
 * Draw a resource tile.
 * @param tile The tile to draw.
 */
void DrawingGameVisitor::visit(ResourceTile& tile) {
	Coordinate coord = tile.getLocation();
	static const GLuint tileTextures = loadImageAsTexture("resources/catan_sprite_sheet.bmp");
	
	glBindTexture(GL_TEXTURE_2D, tileTextures);
	static const std::map<resourceType, pair<float, float>> topRightPoints = {
		make_pair(WOOD, make_pair(260.f, 17.f)),
		make_pair(BRICK, make_pair(629.f, 17.f)),
		make_pair(STONE, make_pair(262.f, 282.f)),
		make_pair(SHEEP, make_pair(621.f, 286.f)),
		make_pair(WHEAT, make_pair(267.f, 554.f)),
		make_pair(DESERT, make_pair(620.f, 555.f))
	};
	static const std::vector<pair<float, float>> resourceTexOffsets = {
		make_pair(0.f, 0.f),
		make_pair(87.f, 130.f),
		make_pair(0.f, 261.f),
		make_pair(-174.f, 261.f),
		make_pair(-260.f, 130.f),
		make_pair(-175, -1)
	};
	static const std::map<int, pair<float, float>> numberTexPoints = {
		make_pair(2, make_pair(1238.5f, 70.5f)),
		make_pair(3, make_pair(1365.5f, 70.5f)),
		make_pair(4, make_pair(1238.5f, 178.5f)),
		make_pair(5, make_pair(1365.5f, 178.5f)),
		make_pair(6, make_pair(1238.5f, 286.5f)),
		make_pair(8, make_pair(1365.5f, 286.5f)),
		make_pair(9, make_pair(1238.5f, 404.5f)),
		make_pair(10, make_pair(1365.5f, 404.5f)),
		make_pair(11, make_pair(1238.5f, 515.5f)),
		make_pair(12, make_pair(1365.5f, 515.5f))
	};
	static const float radius = 59.5f;
	static Coordinate adjacentCoordDiffs[] = {Coordinate(0, 1), Coordinate(1, 0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0), Coordinate(-1, 1)};
	auto topRightPoint = topRightPoints.find(tile.getType());
	if(topRightPoint == topRightPoints.end()) {
		throw runtime_error("Cannot draw this tile; it is invalid.");
	}
	glColor3d(1.0, 1.0, 1.0);
	
	glBegin(GL_TRIANGLE_FAN);
	auto average = averagePoint(resourceTexOffsets);
	texCoordPair({average.first + topRightPoint->second.first, average.second + topRightPoint->second.second});
	vertexPair(coord);
	for(unsigned int i = 0; i < resourceTexOffsets.size(); i++) {
		texCoordPair(std::make_pair(resourceTexOffsets[i].first + topRightPoint->second.first, resourceTexOffsets[i].second + topRightPoint->second.second));
		Coordinate diff = adjacentCoordDiffs[i];
		vertexPair(Coordinate(coord.first + diff.first, coord.second + diff.second));
	}
	texCoordPair(topRightPoint->second);
	vertexPair(Coordinate(coord.first + adjacentCoordDiffs[0].first, coord.second + adjacentCoordDiffs[0].second));
	glEnd();
	
	if(tile.getDiceValue() != 0) {
		drawTexturedCircle(numberTexPoints.find(tile.getDiceValue())->second, radius, coordToScreen(coord), 0.04);
	}
	glBindTexture(GL_TEXTURE_2D, 0);


	

	
}

/**
 * Draw a development card.
 * @param card The development card to draw.
 */
void DrawingGameVisitor::visit(DevelopmentCard& card) {
	
}

/**
 * Construct a trading view.
 * @param initiating The player initiating the trade.
 * @param receiving The other player involved in the trade.
 * @param trade The callback for the "trade" button, invoked with the current offer.
 * @param cancel The callback for the "cancel" button
 * @param initialOffer The initial offer to display.
 */
TradingView::TradingView(Player& initiating, Player& receiving, std::function<bool(std::array<int, 5>, ScreenCoordinate)> trade, std::function<bool(ScreenCoordinate)> cancel, std::array<int, 5> initialOffer) : 
	ViewElement({getGraphicsConfig()["screen.tradingView.bottomLeft"], getGraphicsConfig()["screen.tradingView.topRight"]}),
	initiating(initiating), 
	receiving(receiving),
	trade(
		std::bind(trade, std::ref(offer), std::placeholders::_1), 
		{getGraphicsConfig()["screen.tradingView.tradeButton.bottomLeft"], getGraphicsConfig()["screen.tradingView.tradeButton.topRight"]},
		getGraphicsConfig()["font.path"], getGraphicsConfig()["font.size"], "Trade"
	),
	cancel(cancel, 
		{getGraphicsConfig()["screen.tradingView.cancelButton.bottomLeft"], getGraphicsConfig()["screen.tradingView.cancelButton.topRight"]},
		getGraphicsConfig()["font.path"], getGraphicsConfig()["font.size"], "Cancel"
	),
	offer(initialOffer) {
}

/**
 * Destroy the TradingView.
 */
TradingView::~TradingView() {
	
}

/**
 * Handle a user clicking on the TradingView.
 * @param coord The coordinate clicked on.
 */
bool TradingView::clicked(ScreenCoordinate coord) {
	if(cancel.handleClick(coord)) {
		return true;
	} else if(trade.handleClick(coord)) {
		return true;
	}
	int modifier = coord.first <= 0.5 ? -1 : 1;
	int resource = (coord.second - getGraphicsConfig()["screen.tradingView.resources.bottomY"]) / getGraphicsConfig()["screen.tradingView.resources.height"];
	if(resource >= 0 && resource <= 5) {
		offer[resource] += modifier;
	}
	return true;
}

/**
 * Render the TradingView.
 */
void TradingView::render() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3d(1, 1, 0);
	auto topLeft = ViewElement::getRect().first;
	auto bottomRight = ViewElement::getRect().second;
	glBegin(GL_QUADS);
	glVertex2f(topLeft.first, topLeft.second);
	glVertex2f(bottomRight.first, topLeft.second);
	glVertex2f(bottomRight.first, bottomRight.second);
	glVertex2f(topLeft.first, bottomRight.second);
	glEnd();
	
	auto font = getGraphicsConfig()["font.path"];
	auto fontSize = getGraphicsConfig()["font.size"];
	
	std::string resources[] = {"Wood", "Brick", "Ore", "Wheat", "Wool"};
	for(int i = 0; i < 5; i++) {
		auto leftX = getGraphicsConfig()["screen.tradingView.resources.leftX"];
		auto rightX = getGraphicsConfig()["screen.tradingView.resources.rightX"];
		auto height = getGraphicsConfig()["screen.tradingView.resources.height"];
		auto bottomY = getGraphicsConfig()["screen.tradingView.resources.bottomY"];
		renderText(font, fontSize, {leftX, bottomY + (i * height)}, {rightX, bottomY + height + (i * height)}, toString(offer[i]) + " " + resources[i]);
	}
	auto playersBottomLeft = getGraphicsConfig()["screen.tradingView.players.bottomLeft"];
	auto playersTopRight = getGraphicsConfig()["screen.tradingView.players.topRight"];
	renderText(font, fontSize, playersBottomLeft, playersTopRight, initiating.getName() + " -> " + receiving.getName());
	
	cancel.render();
	trade.render();
}



ConfirmationDialogue::ConfirmationDialogue(std::function<bool(ScreenCoordinate)> confirm_action, std::function<bool(ScreenCoordinate)> cancel_action,
		std::pair<ScreenCoordinate, ScreenCoordinate> rect, std::string message): ViewElement(rect), message(message){
	topLeft = ViewElement::getRect().first;
	bottomRight = ViewElement::getRect().second;

	float width = bottomRight.first - topLeft.first;
	float height = bottomRight.second - topLeft.second;
	ScreenCoordinate confirmTopLeft = ScreenCoordinate(topLeft.first +(width*.1), topLeft.second+(height*.1));
	ScreenCoordinate confirmBottomRight = ScreenCoordinate(bottomRight.first - (width * .6), bottomRight.second - (height * .6));
	ScreenCoordinate cancelTopLeft = ScreenCoordinate(topLeft.first + (width*.6), topLeft.second + (height *.1));
	ScreenCoordinate cancelBottomRight = ScreenCoordinate(bottomRight.first - (width * .1), bottomRight.second - (height * .6));
	
	auto font = getGraphicsConfig()["font.path"];
	auto fontSize = getGraphicsConfig()["font.size"];
	
	confirmButton = std::unique_ptr<ViewElement>(new ViewButtonText(confirm_action, {confirmTopLeft, confirmBottomRight}, font, fontSize, "Yes"));
	cancelButton = std::unique_ptr<ViewElement>(new ViewButtonText(cancel_action, {cancelTopLeft, cancelBottomRight}, font, fontSize, "No"));
}

bool ConfirmationDialogue::clicked(ScreenCoordinate coord){
	if(confirmButton->handleClick(coord)){
		return true;
	} else if(cancelButton->handleClick(coord)){
		return true;
	}
	return false;
}

void ConfirmationDialogue::render(){
	glBindTexture(GL_TEXTURE_2D, 0);

	glColor3f(1., 1., 1.);
	glBegin(GL_QUADS);
	glVertex2f(topLeft.first, topLeft.second);
	glVertex2f(bottomRight.first, topLeft.second);
	glVertex2f(bottomRight.first, bottomRight.second);
	glVertex2f(topLeft.first, bottomRight.second);
	glEnd();
	
	auto font = getGraphicsConfig()["font.path"];
	auto fontSize = getGraphicsConfig()["font.size"];
	
	float width = bottomRight.first - topLeft.first;
	float height = bottomRight.second - topLeft.second;

	renderText(font, fontSize, {topLeft.first + .05*width, topLeft.second + .4*height}, {bottomRight.first - .05*width, bottomRight.second - .15*height}, message);

	glColor3f(0., 1., 0.);
	confirmButton->render();

	glColor3f(1.,0.,0.);
	cancelButton->render();
}





