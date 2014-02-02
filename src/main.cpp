#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>


void initOpenGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/* function to reset our viewport after a window resize */
int updateViewport( int width, int height ) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0, 1, 0, 1);
	
	glMatrixMode(GL_MODELVIEW);
}

void render(SDL_Renderer* displayRenderer) {
	
	
	SDL_RenderPresent(displayRenderer);
}


int
main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* displayWindow;
	SDL_Renderer* displayRenderer;
	SDL_RendererInfo displayRendererInfo;
	SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
	SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
	/*TODO: Check that we have OpenGL */
	if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 || 
		(displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
		/*TODO: Handle this. We have no render surface and not accelerated. */
		std::cout << "Unable to create a window using accelerated graphics." << std::endl;
	}
	
	
	initOpenGL();

	updateViewport(800, 600);
	
	render(displayRenderer);
	
	
	SDL_Delay(5000);
	SDL_Quit();
	
	return 0;
}