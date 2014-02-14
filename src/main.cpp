#define _USE_MATH_DEFINES
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <utility>

enum PieceType {
	NO_PIECE, WHITE_PIECE, BLACK_PIECE
};

PieceType board[8][8];

void initGame() {
	for(int row = 0; row < 3; row++) {
		for(int col = 0; col < 8; col++) {
			board[row][col] = (row + col) % 2 == 0 ? NO_PIECE : WHITE_PIECE;
		}
	}
	for(int row = 3; row < 5; row++) {
		for(int col = 0; col < 8; col++) {
			board[row][col] = NO_PIECE;
		}
	}
	for(int row = 5; row < 8; row++) {
		for(int col = 0; col < 8; col++) {
			board[row][col] = (row + col) % 2 == 0 ? NO_PIECE : BLACK_PIECE;
		}
	}
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

std::pair<int, int> windowToGameCoords(int x, int y) {
	return std::pair<int, int>((8. * x / 800.), (8. * y / 600.));
}

std::pair<int, int> selectedCoord(-1, -1);

bool handleMouseButtonEvent(const SDL_MouseButtonEvent& event) {
	std::pair<int, int> gameCoords = windowToGameCoords(event.x, event.y);
	
	if(event.type == SDL_MOUSEBUTTONDOWN) {
		std::cout << "mouse click on " << gameCoords.first << " " << gameCoords.second << std::endl;
		if(selectedCoord == std::make_pair(-1, -1)) {
			selectedCoord = gameCoords;
		} else {
			board[gameCoords.second][gameCoords.first] = board[selectedCoord.second][selectedCoord.first];
			board[selectedCoord.second][selectedCoord.first] = NO_PIECE;
			selectedCoord = std::make_pair(-1, -1);
		}
	}
	
	return true;
}

bool handleKeyboardEvent(const SDL_KeyboardEvent& event) {
	if(event.keysym.sym == SDLK_ESCAPE) {
		return false;
	}
	//TODO: key-specific code
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

void drawCircle(float x, float y, float radius) {
	const int vertices = 20;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for(int i = 0; i < vertices + 1; i++) {
		glVertex2f(x + radius * cos(2.0 * M_PI * i / vertices), y + radius * sin(2.0 * M_PI * i / vertices));
	}
	glEnd();
}

void render(SDL_Window* displayWindow) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for(int row = 0; row < 8; row++) {
		for(int col = 0; col < 8; col++) {
			if(board[row][col] == WHITE_PIECE) {
				glColor3f(1, 1, 1);
			} else if(board[row][col] == BLACK_PIECE) {
				glColor3f(0, 0, 0);
			}
			
			if(board[row][col] != NO_PIECE) {
				drawCircle((col + .5) / 8., (row + .5) / 8., 1/16.);
			}
		}
	}
	
	glBegin(GL_QUADS);
	for(int row = 0; row < 8; row++) {
		for(int col = 0; col < 8; col++) {
			float shade = (row + col) % 2 == 0 ? .75f : .25f;
			glColor3f(shade, shade, shade);
			if(selectedCoord == std::make_pair(row, col)) {
				glColor3f(1.f, 0.f, 0.f);
			}
			glVertex2f((row + 0) / 8.f, (col + 0) / 8.f);
			glVertex2f((row + 1) / 8.f, (col + 0) / 8.f);
			glVertex2f((row + 1) / 8.f, (col + 1) / 8.f);
			glVertex2f((row + 0) / 8.f, (col + 1) / 8.f);
		}
	}
	glEnd();
	
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