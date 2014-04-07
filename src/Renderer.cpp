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

/**
 * Loads an image into an OpenGL texture.
 * @param name The file name of the texture to load.
 * @return An OpenGL texture.
 */
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

/**
 * Convert a triangular game coordinate to an x-y screen cordinate (from 0 to 1).
 * @param coord The game coordinate to convert.
 * @return The coordinate on screen to draw to.
 */
pair<float, float> coordToScreen(const Coordinate& coord) {
	using std::sin;
	using std::cos;
	// TODO not magic numbers
	static const float scale = 0.1f;
	static const float angle = M_PI / 3.f;
	float x = .25f + (scale * coord.first) + ((scale * coord.second) * cos(angle));
	float y = .1f + (scale * coord.second) * sin(angle);
	return std::make_pair(x, y);
}

/**
 * Convert a screen x-y coordinate (from 0 to 1) to a game coordinate.
 * @param screen The screen coordinate to convert.
 * @return The game coordinate.
 */
Coordinate screenToCoord(const pair<float, float>& screen) {
	static const float scale = 0.1;
	static const float angle = M_PI / 3.f;
	Coordinate ret;
	float y_approx = (screen.second - 0.1f) / std::sin(angle) / scale;
	ret.second = std::round(y_approx);
	ret.first = std::round((screen.first - 0.2f) / scale - (screen.second - 0.1f) / scale / std::sin(angle) * std::cos(angle)) - 1;
	return ret;
}

/**
 * Draw a game coordinate as a vertex.
 * @param coord The game coordinate to draw.
 */
void vertexPair(const Coordinate& coord) {
	pair<float, float> screenCoord = coordToScreen(coord);
	glVertex2f(screenCoord.first, screenCoord.second);
}

/**
 * Take a texture coordinate (x between 0 and 2048 and y between 0 and 1024, corresponding to the sprite sheet).
 */
void texCoordPair(const pair<float, float>& p) {
	glTexCoord2f(p.first / 2048., p.second / 1024.);
}

/**
 * The average point in a vector of points.
 * @param points The points to aggregate.
 * @return The average of the points.
 */
pair<float, float> averagePoint(const std::vector<pair<float, float>>& points) {
	pair<float, float> average;
	for(auto& it : points) {
		average.first += it.first;
		average.second += it.second;
	}
	average.first /= (float) points.size();
	average.second /= (float) points.size();
	return average;
}