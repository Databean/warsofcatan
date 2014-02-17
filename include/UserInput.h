#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <SDL2/SDL.h>

#include "GameBoard.h"
#include "Player.h"

bool acceptInput(GameBoard& board, Player& turn, SDL_Event& event);

#endif