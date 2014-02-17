#include "Renderer.h"

#include <GL/gl.h>

#include "GameBoard.h"
#include "Player.h"

void renderBoard(GameBoard& board, Player& perspective) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glFlush();
}
