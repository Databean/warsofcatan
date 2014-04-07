#define _USE_MATH_DEFINES
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "SDL_ttf.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <utility>
#include <vector>
#include <memory>

#include "GameBoard.h"
#include "Player.h"
#include "GameView.h"
#include "GameController.h"

using std::vector;
using std::unique_ptr;

/**
 * Initialize some OpenGL settings.
 */
void initOpenGL() {
	glEnable(GL_TEXTURE_2D);
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glDepthFunc(GL_NEVER);
}

/**
 * Reset the viewport after a window resize.
 * @param width The new width of the viewport.
 * @param height The new height of the viewport.
 */
void updateViewport(int width, int height) {
	glViewport(0, 0, width, height); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0, 1, 0, 1);
	
	glMatrixMode(GL_MODELVIEW);
}

/**
 * Main. Initializes SDL and the model, view, and controller. Also has the main game loop.
 */
int main(int argc, char *argv[]) {
	const int windowWidth = 900, windowHeight = 800;
	
	if(TTF_Init()==-1) {
		std::cout << "Error in TTF_Init: " << TTF_GetError() << std::endl;
		return 2;
	}
	
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	SDL_Window* displayWindow;
	SDL_Renderer* displayRenderer;
	SDL_RendererInfo displayRendererInfo;
	SDL_CreateWindowAndRenderer(windowWidth, windowHeight, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
	SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
	/*TODO: Check that we have OpenGL */
	if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 || 
		(displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
		std::cout << "Unable to create a window using accelerated graphics." << std::endl;
		return 0;
	}
	
	SDL_GLContext glContext = SDL_GL_CreateContext(displayWindow);
	
	initOpenGL();

	updateViewport(windowWidth, windowHeight);
	
	vector<unique_ptr<Player>> players;
	players.emplace_back(unique_ptr<Player>(new Player("test")));
	
	Player& firstPlayer = *players[0];
	
	GameBoard model(std::move(players));
	GameView view(model);
	GameController controller(model, view);
	
	model.PlaceSettlement(Coordinate{0, 0}, firstPlayer);
	model.PlaceRoad(Coordinate{0, 0}, Coordinate{1, 0}, firstPlayer);
	model.PlaceRoad(Coordinate{1, 0}, Coordinate{1, 1}, firstPlayer);
	model.PlaceRoad(Coordinate{1, 1}, Coordinate{0, 2}, firstPlayer);
	model.PlaceSettlement(Coordinate{0, 2}, firstPlayer);
	model.UpgradeSettlement(Coordinate{0, 2});
	
	bool running = true;
	while(running) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			running = view.acceptInput(event);
		}
		
		view.render();
		
		SDL_GL_SwapWindow(displayWindow);
		SDL_Delay(100);
	}
	
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(displayWindow);
	SDL_DestroyRenderer(displayRenderer);
	SDL_Quit();
	
	return 0;
}
