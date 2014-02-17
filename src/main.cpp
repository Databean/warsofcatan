#define _USE_MATH_DEFINES
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <utility>

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
int updateViewport(int width, int height) {
	glViewport(0, 0, width, height); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0, 1, 1, 0);
	
	glMatrixMode(GL_MODELVIEW);
}

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
bool handleEvent(const SDL_Event& event) {
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

void render(SDL_Window* displayWindow) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glFlush();
	SDL_GL_SwapWindow(displayWindow);
}


int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* displayWindow;
	SDL_Renderer* displayRenderer;
	SDL_RendererInfo displayRendererInfo;
	SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
	SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
	/*TODO: Check that we have OpenGL */
	if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 || 
		(displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
		std::cout << "Unable to create a window using accelerated graphics." << std::endl;
		return 0;
	}
	
	SDL_GLContext glContext = SDL_GL_CreateContext(displayWindow);
	
	initGame();
	initOpenGL();

	updateViewport(800, 600);
	
	bool running = true;
	while(running) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			running = handleEvent(event);
		}
		render(displayWindow);
		SDL_Delay(100);
	}
	
	SDL_GL_DeleteContext(glContext);
	SDL_Quit();
	
	return 0;
}