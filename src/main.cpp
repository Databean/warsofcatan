#define _USE_MATH_DEFINES
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <utility>
#include <vector>
#include <memory>

#include "Config.h"
#include "GameBoard.h"
#include "Player.h"
#include "GameView.h"
#include "GameController.h"
#include "Util.h"

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
 * Run the game loop until the game exits.
 * @param displayWindow The window the game is being drawn in.
 * @param view The view of the game being drawn.
 */
void gameLoop(SDL_Window& displayWindow, GameView& view) {
	bool running = true;
	while(running) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			running = view.acceptInput(event);
		}
		
		view.render();
		
		SDL_GL_SwapWindow(&displayWindow);
		SDL_Delay(100);
	}
}

/**
 * Check the renderer created by SDL is valid for our purposes (uses accelerated graphics).
 * @param displayRenderer The renderer being used for the game.
 * @return If the renderer is valid.
 */
bool checkRenderer(SDL_Renderer& displayRenderer) {
	SDL_RendererInfo displayRendererInfo;
	SDL_GetRendererInfo(&displayRenderer, &displayRendererInfo);
	return (displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) && (displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE);
}

/**
 * Initialize the game objects, and run the game loop.
 */
void game() {
	auto displayWindow = make_resource(SDL_CreateWindow, SDL_DestroyWindow, 
		"Wars of Catan", 
		(float)getGraphicsConfig()["screen.x"], 
		(float)getGraphicsConfig()["screen.y"], 
		(float)getGraphicsConfig()["screen.width"], 
		(float)getGraphicsConfig()["screen.height"], 
		SDL_WINDOW_OPENGL);
	
	auto displayRenderer = make_resource(SDL_CreateRenderer, SDL_DestroyRenderer, 
		displayWindow.get(), 
		-1, 
		SDL_RENDERER_ACCELERATED);
	
	if(checkRenderer(*displayRenderer)) {}
	else {
		std::cerr << "Unable to create a rendering window using accelerated graphics." << std::endl;
		return;
	}
	
	SDL_GLContext glContext = SDL_GL_CreateContext(displayWindow.get());
	
	initOpenGL();
	
	updateViewport(getGraphicsConfig()["screen.width"], getGraphicsConfig()["screen.height"]);
	
	GameBoard model({"Southern Tribes", "Western Watch", "North Guard", "East Raiders"});
	GameView view(model);
	GameController controller(model, view);
	
	model.initializeGame();
	
	gameLoop(*displayWindow, view);
	
	SDL_GL_DeleteContext(glContext);
}

/**
 * Main. Initializes SDL and the model, view, and controller. Also has the main game loop.
 */
int main(int argc, char *argv[]) {
	
	if(TTF_Init()==-1) {
		std::cerr << "Error in TTF_Init: " << TTF_GetError() << std::endl;
		return 2;
	}
	
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) {
		std::cerr << "Error in SDL_Init: " << SDL_GetError() << std::endl;
		return -1;
	}
	
	game();
	
	SDL_Quit();
}
