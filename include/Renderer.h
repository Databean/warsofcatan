#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

#include "GameBoard.h"
#include "Player.h"

void renderBoard(const GameBoard& board, const Player& perspective);

GLuint loadImageAsTexture(const std::string& name);

std::pair<float, float> coordToScreen(const Coordinate& coord);
Coordinate screenToCoord(const std::pair<float, float>&);

void vertexPair(const Coordinate& coord);

void texCoordPair(const std::pair<float, float>& p);

std::pair<float, float> averagePoint(const std::vector<std::pair<float, float>>& points);

#endif