#include "UserInput.h"

#include <SDL2/SDL.h>

#include "GameBoard.h"
#include "Player.h"

bool handleMouseButtonEvent(const SDL_MouseButtonEvent& event) {
	
	return true;
}

bool handleKeyboardEvent(const SDL_KeyboardEvent& event) {
	
	return true;
}

bool handleQuitEvent(const SDL_QuitEvent& event) {
	return false;
}

// Returns true if the program should continue running, false if it should exit
bool acceptInput(GameBoard& board, Player& turn, SDL_Event& event) {
	switch(event.type) {
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			return handleMouseButtonEvent(event.button);
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			return handleKeyboardEvent(event.key);
		case SDL_QUIT:
			return handleQuitEvent(event.quit);
	}
	return true;
}
