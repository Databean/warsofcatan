#include "Renderer.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <utility>
#include <stdexcept>
#include <string>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

#include "GameBoard.h"
#include "Player.h"

using std::make_pair;
using std::pair;
using std::runtime_error;
using std::string;

GLuint loadImageAsTexture(const string& name) {
	SDL_Surface* imageSurface = SDL_LoadBMP(name.c_str());
	if(imageSurface == nullptr) {
		string error = SDL_GetError();
		SDL_ClearError();
		throw runtime_error("Unable to load image: " + error);
	}
	// Adapted from http://stackoverflow.com/questions/18934865/sdl2-with-opengl-texture-displaying-incorrectly
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSurface->w, imageSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, imageSurface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	SDL_FreeSurface(imageSurface);
	
	return texture;
}

inline pair<float, float> coordToScreen(const Coordinate& coord) {
	using std::sin;
	using std::cos;
	// TODO not magic numbers
	float scale = 0.1f;
	float angle = M_PI / 3.f;
	float x = .5f + (scale * coord.first) + (scale * coord.second) * cos(angle);
	float y = .5f + (scale * coord.second) * sin(angle);
	return std::make_pair(x, y);
}

inline void vertexPair(const Coordinate& coord) {
	pair<float, float> screenCoord = coordToScreen(coord);
	glVertex2f(screenCoord.first, screenCoord.second);
}

inline void texCoordPair(const pair<float, float>& p) {
	glTexCoord2f(p.first / 2048., p.second / 2048.);
}

inline pair<float, float> averagePoint(const std::vector<pair<float, float>>& points) {
	pair<float, float> average;
	for(auto& it : points) {
		average.first += it.first;
		average.second += it.second;
	}
	average.first /= (float) points.size();
	average.second /= (float) points.size();
	return average;
}

void drawHex(const Coordinate& coord, const ResourceTile& tile) {
	static GLuint tileTextures = loadImageAsTexture("resources/tiles.bmp");
	glBindTexture(GL_TEXTURE_2D, tileTextures);
	typedef std::vector<pair<float, float> > texCoordList;
	static const std::map<ResourceTile::Type, texCoordList> resourceTexCoords = {
		make_pair(ResourceTile::GRAIN, texCoordList { make_pair(377, 73), make_pair(500, 285), 
			make_pair(380, 502), make_pair(136, 503), make_pair(10, 288), make_pair(134, 74)}),
		make_pair(ResourceTile::SHEEP, texCoordList { make_pair(959, 75), make_pair(1076, 288),
			make_pair(955, 503), make_pair(712, 501), make_pair(586, 289), make_pair(708, 73)}),
		make_pair(ResourceTile::WOOD, texCoordList { make_pair(1491, 60), make_pair(1618, 269),
			make_pair(1479, 490), make_pair(1260, 493), make_pair(1126, 283), make_pair(1246, 65)}),
		make_pair(ResourceTile::ORE, texCoordList { make_pair(382, 689), make_pair(506, 898),
			make_pair(386, 1118), make_pair(142, 1120), make_pair(17, 905), make_pair(138, 691)}),
		make_pair(ResourceTile::BRICK, texCoordList { make_pair(1496, 690), make_pair(1617, 908),
			make_pair(1490, 1120), make_pair(1248, 1118), make_pair(1124, 898), make_pair(1250, 688)}),
		make_pair(ResourceTile::DESERT, texCoordList { make_pair(1496, 690), make_pair(1617, 908),
			make_pair(1490, 1120), make_pair(1248, 1118), make_pair(1124, 898), make_pair(1250, 688)}),
	};
	static Coordinate adjacentCoordDiffs[] = {Coordinate(0, 1), Coordinate(1, 0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0), Coordinate(-1, 1)};
	if(resourceTexCoords.find(tile.getType()) == resourceTexCoords.end()) {
		throw runtime_error("Cannot draw this tile; it is invalid.");
	}
	const texCoordList& texCoords = resourceTexCoords.at(tile.getType());
	glBegin(GL_TRIANGLE_FAN);
	texCoordPair(averagePoint(texCoords));
	vertexPair(coord);
	for(unsigned int i = 0; i < texCoords.size(); i++) {
		texCoordPair(texCoords[i]);
		Coordinate diff = adjacentCoordDiffs[i];
		vertexPair(Coordinate(coord.first + diff.first, coord.second + diff.second));
	}
	texCoordPair(texCoords[0]);
	vertexPair(Coordinate(coord.first + adjacentCoordDiffs[0].first, coord.second + adjacentCoordDiffs[0].second));
	glEnd();
	/*
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(1, 0); glVertex2f(1, 0);
	glTexCoord2f(1, 1); glVertex2f(1, 1);
	glTexCoord2f(0, 1); glVertex2f(0, 1);
	glEnd();
	*/
	glBindTexture(GL_TEXTURE_2D, 0);
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
