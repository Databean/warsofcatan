#include "GameView.h"

#define _USE_MATH_DEFINES

#include <stdexcept>
#include <cmath>

#include "GameBoard.h"
#include "GameController.h"
#include "Renderer.h"
#include "City.h"

using std::make_pair;
using std::pair;
using std::runtime_error;
using std::string;

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
 * Display the GameBoard to the screen as well as additional ViewElements.
 */
void GameView::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DrawingGameVisitor visitor(*this);
	model.accept(visitor);
	
	for(auto& it : viewElements) {
		it->render();
	}
	
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
			if(it->handleClick(screen)) {
				break;
			}
		}
	}
	return true;
}

/**
 * Add a ViewElement to be rendered and accept input.
 * @param element An owning pointer to the element to be added.
 */
void GameView::addElement(std::unique_ptr<ViewElement> element) {
	viewElements.emplace_back(std::move(element));
}

/*
 * Construct a DrawingGameVisitor with the view that it is drawing to.
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
	
	glColor3d(0., 0., 0.);
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
	glColor3d(0., 0., 0.);
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
	glColor3d(0., 0., 0.);
	glBegin(GL_QUADS);
	glVertex2d(centerScreenPos.first + cityRadius, centerScreenPos.second + cityRadius);
	glVertex2d(centerScreenPos.first + cityRadius, centerScreenPos.second - cityRadius);
	glVertex2d(centerScreenPos.first - cityRadius, centerScreenPos.second - cityRadius);
	glVertex2d(centerScreenPos.first - cityRadius, centerScreenPos.second + cityRadius);
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
