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

pair<float, float> coordToScreen(const Coordinate& coord) {
	using std::sin;
	using std::cos;
	// TODO not magic numbers
	float scale = 0.1f;
	float angle = M_PI / 3.f;
	float x = .5f + (scale * coord.first) + (scale * coord.second) * cos(angle);
	float y = .5f + (scale * coord.second) * sin(angle);
	return std::make_pair(x - 0.25f, y - 0.4f);
}

void vertexPair(const Coordinate& coord) {
	pair<float, float> screenCoord = coordToScreen(coord);
	glVertex2f(screenCoord.first, screenCoord.second);
}

void texCoordPair(const pair<float, float>& p) {
	glTexCoord2f(p.first / 2048., p.second / 1024.);
}

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