#define _USE_MATH_DEFINES
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <utility>
#include <vector>
#include <memory>

#include "GameBoard.h"
#include "UserInput.h"
#include "Player.h"
#include "Renderer.h"

using std::vector;
using std::unique_ptr;

void initGame() {
	
}

void initOpenGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.f, 1.f, 1.f, 1.f);
	//glDepthFunc(GL_NEVER);
}

/* function to reset our viewport after a window resize */
void updateViewport(int width, int height) {
	glViewport(0, 0, width, height); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0, 1, 0, 1);
	
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	SDL_Window* displayWindow;
	SDL_Renderer* displayRenderer;
	SDL_RendererInfo displayRendererInfo;
	SDL_CreateWindowAndRenderer(900, 800, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
	SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
	/*TODO: Check that we have OpenGL */
	if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 || 
		(displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
		std::cout << "Unable to create a window using accelerated graphics." << std::endl;
		return 0;
	}
	
	SDL_GLContext glContext = SDL_GL_CreateContext(displayWindow);
	
	initOpenGL();

	updateViewport(1024, 768);
	
	vector<unique_ptr<Player>> players;
	players.emplace_back(unique_ptr<Player>(new Player("test")));
	
	Player& testPlayer = *(players[0]);
	
	GameBoard testBoard(std::move(players));
	
	bool running = true;
	while(running) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			running = acceptInput(testBoard, testPlayer, event);
		}
		
		renderBoard(testBoard, testPlayer);
		
		SDL_GL_SwapWindow(displayWindow);
		SDL_Delay(100);
	}
	
	SDL_GL_DeleteContext(glContext);
	SDL_Quit();
	
	return 0;
}
