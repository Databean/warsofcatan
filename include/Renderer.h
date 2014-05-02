#ifndef RENDERER_H
#define RENDERER_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

#include "GameBoard.h"
#include "Player.h"

void renderBoard(const GameBoard& board, const Player& perspective);

GLuint loadImageAsTexture(const std::string& name);
GLuint loadTextAsTexture(const std::string& font, int fontSize,
		const std::string& text);

void renderText(const std::string& font, int fontSize,
		const std::pair<float, float> bottomLeft,
		const std::pair<float, float> topRight, const std::string& text);
void renderText(const std::pair<float, float> bottomLeft,
		const std::pair<float, float> topRight, const GLuint& texture);
void renderRectangle(const std::pair<float, float> bottomLeft,
		const std::pair<float, float> topRight,
		const std::tuple<float, float, float> color);

void renderTexturedCircle(const std::pair<float, float> texCenter,
		const float texRadius, const std::pair<float, float> screenCenter,
		const float screenRadius, const GLuint& texture, int articulation = 20);
void renderTexturedRectangle(const std::pair<float, float> screenBottomLeft,
		const std::pair<float, float> screenTopRight,
		const std::pair<float, float> texBottomLeft,
		const std::pair<float, float> texTopRight, const GLuint& texture);

std::pair<float, float> coordToScreen(const Coordinate& coord);
Coordinate screenToCoord(const std::pair<float, float>&);

void vertexPair(const Coordinate& coord);

void texCoordPair(const std::pair<float, float>& p);

std::pair<float, float> averagePoint(
		const std::vector<std::pair<float, float>>& points);

#endif
