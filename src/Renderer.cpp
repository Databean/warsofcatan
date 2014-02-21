#include "Renderer.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include <GL/gl.h>

#include <iostream>
#include <utility>

#include "GameBoard.h"
#include "Player.h"

using std::pair;
using std::sin;
using std::cos;

inline pair<float, float> coordToScreen(const Coordinate& coord) {
	// TODO not magic numbers
	float scale = 0.1f;
	float angle = M_PI / 3.f;
	float x = .5f + (scale * coord.first) + (scale * coord.second) * cos(angle);
	float y = .5f + (scale * coord.second) * sin(angle);
	return std::make_pair(x, y);
}

inline void glVertexCoord(const Coordinate& coord) {
	pair<float, float> screenCoord = coordToScreen(coord);
	glVertex2f(screenCoord.first, screenCoord.second);
}

void drawHex(const Coordinate& coord, const ResourceTile& tile) {
	switch(tile.getType()) {
		case ResourceTile::WOOD:
			glColor3d(133. / 255., 66. / 255., 11. / 255.);
			break;
		case ResourceTile::SHEEP:
			glColor3d(191. / 255., 255. / 255., 189. / 255.);
			break;
		case ResourceTile::ORE:
			glColor3d(59. / 255., 59. / 255., 69. / 255.);
			break;
		case ResourceTile::BRICK:
			glColor3d(196. / 255., 0, 0);
			break;
		case ResourceTile::GRAIN:
			glColor3d(255 / 255., 249 / 255., 87 / 255.);
			break;
		case ResourceTile::DESERT:
			glColor3d(255. / 255., 220. / 255., 138. / 138.);
			break;
	}
	Coordinate adjacentCoordDiffs[] = {Coordinate(0, 1), Coordinate(1, 0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0), Coordinate(-1, 1)};
	glBegin(GL_TRIANGLE_FAN);
	glVertexCoord(coord);
	for(const Coordinate& diff : adjacentCoordDiffs) {
		glVertexCoord(Coordinate(coord.first + diff.first, coord.second + diff.second));
	}
	glVertexCoord(Coordinate(coord.first + adjacentCoordDiffs[0].first, coord.second + adjacentCoordDiffs[0].second));
	glEnd();
}

void renderBoard(const GameBoard& board, const Player& perspective) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for(auto& it : board.getPieces()) {
		const Coordinate& coord = it.first;
		const GamePiece& piece = *(it.second);
		if(dynamic_cast<const ResourceTile*>(&piece)) {
			const ResourceTile& tile = static_cast<const ResourceTile&>(piece);
			drawHex(coord, tile);
		}
	}
	
	glFlush();
}
