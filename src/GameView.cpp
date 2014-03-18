#include "GameView.h"

#define _USE_MATH_DEFINES

#include <stdexcept>
#include <cmath>

#include "GameBoard.h"
#include "GameController.h"
#include "Renderer.h"

using std::make_pair;
using std::pair;
using std::runtime_error;
using std::string;

GameView::GameView(GameBoard& model, GameController& controller) : model(model), controller(controller) {
	
}

GameView::~GameView() {
	
}

void GameView::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DrawingGameVisitor visitor(*this);
	model.accept(visitor);
	
	glFlush();
}

bool GameView::acceptInput(SDL_Event& event) {
	if(event.type == SDL_QUIT) {
		return false;
	}
	return true;
}

DrawingGameVisitor::DrawingGameVisitor(GameView& view) : view(view) {
	
}

DrawingGameVisitor::~DrawingGameVisitor() {
	
}

void DrawingGameVisitor::visit(GameBoard& model) {
	
}

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

void DrawingGameVisitor::visit(City& city) {
	
}

void DrawingGameVisitor::visit(Player& player) {
	
}

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

void DrawingGameVisitor::visit(DevelopmentCard& card) {
	
}
