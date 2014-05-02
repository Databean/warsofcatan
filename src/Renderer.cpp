
#include "Renderer.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <utility>
#include <stdexcept>
#include <string>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>

#include "GameBoard.h"
#include "Player.h"

using std::make_pair;
using std::pair;
using std::runtime_error;
using std::string;

/**
 * Create an OpenGL texture based on some text and a font.
 *
 * @param fontPath The path to the font ttf
 * @param text The text to render.
 */
GLuint loadTextAsTexture(const std::string& fontPath, int fontSize, const std::string& text) {
	TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
	if(!font) {
		throw runtime_error("TTF_OpenFont: " + string(TTF_GetError()));
	}

	//Use glColor... if you don't want black text.
	SDL_Color color {0, 0, 0};

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	TTF_CloseFont(font);

	if(!textSurface) {
		throw runtime_error("TTF_RenderText_Solid: " + string(TTF_GetError()));
	}

	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	SDL_Surface* imageSurface = SDL_ConvertSurface(textSurface, format, 0);
	SDL_FreeSurface(textSurface);
	SDL_FreeFormat(format);

	// TTF_RenderText produces ARGB images. OpenGL doesn't take ARGB images, only RGBA images
	// so we have to move the color values around so that OpenGL renders it properly.

	int bpp = imageSurface->format->BytesPerPixel;
	SDL_LockSurface(imageSurface);
	for(int x = 0; x < imageSurface->w; x++) {
		for(int y = 0; y < imageSurface->h; y++) {
			Uint8 *p = (Uint8 *)imageSurface->pixels + y * imageSurface->pitch + x * bpp;
			// Starts out as ARGB.
			std::swap(p[0], p[1]); //RAGB
			std::swap(p[1], p[2]); //RGAB
			std::swap(p[2], p[3]); //RGBA
		}
	}

	SDL_UnlockSurface(imageSurface);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSurface->w, imageSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSurface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	SDL_FreeSurface(imageSurface);

	return texture;
}

/**
 * Render some text on screen.
 *
 * @param fontPath The path to the font ttf
 * @param fontSize The size to draw the letters in.
 * @param bottomLeft The bottom left screen coordinate of the bounding box to draw to.
 * @param topRight The top right screen coordinate of the bounding box to draw to.
 * @param text The text to render.
 */
void renderText(const std::string& fontPath, int fontSize, const std::pair<float, float> bottomLeft, const std::pair<float, float> topRight, const std::string& text) {

	GLuint texture = loadTextAsTexture(fontPath, fontSize, text);

	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(bottomLeft.first, bottomLeft.second);
	glTexCoord2f(0, 0);
	glVertex2f(bottomLeft.first, topRight.second);
	glTexCoord2f(1, 0);
	glVertex2f(topRight.first, topRight.second);
	glTexCoord2f(1, 1);
	glVertex2f(topRight.first, bottomLeft.second);
	glEnd();

	glDeleteTextures(1, &texture);
}

/**
 * Render some text on screen.
 *
 * @param bottomLeft The bottom left screen coordinate of the bounding box to draw to.
 * @param topRight The top right screen coordinate of the bounding box to draw to.
 * @param texture The texture of text to render.
 */
void renderText(const std::pair<float, float> bottomLeft, const std::pair<float, float> topRight, const GLuint& texture){
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 1);
	glVertex2f(bottomLeft.first, bottomLeft.second);
	glTexCoord2i(1, 1);
	glVertex2f(topRight.first, bottomLeft.second);
	glTexCoord2i(1, 0);
	glVertex2f(topRight.first, topRight.second);
	glTexCoord2i(0, 0);
	glVertex2f(bottomLeft.first, topRight.second);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * Render a colored rectangle on the screen
 *
 * @param bottomLeft the bottom left corner of the box
 * @param topRight the top right corner of the box
 * @param color the rgb color percentages of the box
 */
void renderRectangle(const std::pair<float, float> bottomLeft, const std::pair<float, float> topRight, const std::tuple<float, float, float> color){
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(std::get<0>(color), std::get<1>(color), std::get<2>(color));

	glBegin(GL_QUADS);
	glVertex2f(bottomLeft.first, bottomLeft.second);
	glVertex2f(topRight.first, bottomLeft.second);
	glVertex2f(topRight.first, topRight.second);
	glVertex2f(bottomLeft.first, topRight.second);
	glEnd();
}

/**
 * Convenience method to draw a circle that corresponds to a circular texture in the texture file.
 * @param texCenter The center of the texture.
 * @param texRadius The radius of the texture.
 * @param screenCenter The center of the circle drawn on screen.
 * @param screenRadius The radius of the circle drawn on screen.
 * @param articulation The number of points to draw in the circle.
 */
void renderTexturedCircle(const std::pair<float, float> texCenter, const float texRadius, const std::pair<float, float> screenCenter,
		const float screenRadius, const GLuint& texture, int articulation) {

	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3d(1.0, 1.0, 1.0);

	glBegin(GL_TRIANGLE_FAN);
	texCoordPair(texCenter);
	glVertex2f(screenCenter.first, screenCenter.second);
	for(int i = 0; i < articulation + 1; i++) {
		double angle = ((double) i) * (2. * M_PI) / (double)articulation;
		double tangle = ((double) -i) * (2. * M_PI) / (double)articulation;
		texCoordPair({texCenter.first + texRadius * std::cos(tangle), texCenter.second + texRadius * std::sin(tangle)});
		glVertex2d(screenCenter.first + (screenRadius * std::cos(angle)), screenCenter.second + (screenRadius * std::sin(angle)));

	}
	glEnd();


	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * Draw a textured square oriented parallel to the ground
 * @param texTopLeft image coordinates of the top left side of our square texture
 * @param sideLength image domain side length (in pixels)
 * @param screenTopLeft GL coordinates for the top left of our square to render
 * @param screenSideLength GL image domain square side length
 */
void renderTexturedRectangle(const std::pair<float, float> screenBottomLeft, const std::pair<float, float> screenTopRight,
		const std::pair<float, float> texBottomLeft, const std::pair<float, float> texTopRight, const GLuint& texture){


	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3d(1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
	texCoordPair({texBottomLeft.first, texBottomLeft.second});
	glVertex2d(screenBottomLeft.first, screenBottomLeft.second);

	texCoordPair({texTopRight.first, texBottomLeft.second});
	glVertex2d(screenTopRight.first, screenBottomLeft.second);

	texCoordPair({texTopRight.first, texTopRight.second});
	glVertex2d(screenTopRight.first, screenTopRight.second);

	texCoordPair({texBottomLeft.first, texTopRight.second});
	glVertex2d(screenBottomLeft.first, screenTopRight.second);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}



/**
 * Loads an image into an OpenGL texture.
 * @param name The file name of the texture to load.
 * @return An OpenGL texture.
 */
GLuint loadImageAsTexture(const string& name) {
	SDL_Surface* imageSurface = SDL_LoadBMP(name.c_str());
	if(imageSurface == nullptr) {
		string error = SDL_GetError();
		SDL_ClearError();
		throw runtime_error("Unable to load image: " + error);
	}
	// Adapted from http://stackoverflow.com/questions/18934865/sdl2-with-opengl-texture-displaying-incorrectly
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSurface->w, imageSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, imageSurface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(imageSurface);

	return texture;
}

/**
 * Convert a triangular game coordinate to an x-y screen cordinate (from 0 to 1).
 * @param coord The game coordinate to convert.
 * @return The coordinate on screen to draw to.
 */
pair<float, float> coordToScreen(const Coordinate& coord) {
	using std::sin;
	using std::cos;
	// TODO not magic numbers
	static const float xscale = 9.f / 16.f / 6.f;
	static const float yscale = 0.1f;
	static const float angle = M_PI / 3.f;
	float x = .25f + (xscale * coord.first) + ((yscale * coord.second) * cos(angle));
	float y = .1f + (yscale * coord.second) * sin(angle);
	return std::make_pair(x, y);
}

/**
 * Convert a screen x-y coordinate (from 0 to 1) to a game coordinate.
 * @param screen The screen coordinate to convert.
 * @return The game coordinate.
 */
Coordinate screenToCoord(const pair<float, float>& screen) {
	static const float xscale = 9.f / 16.f / 6.f;
	static const float yscale = 0.1f;
	static const float angle = M_PI / 3.f;
	Coordinate ret;
	float y_approx = (screen.second - 0.1f) / std::sin(angle) / yscale;
	ret.second = std::round(y_approx);
	ret.first = std::round((screen.first - 0.25f - y_approx * yscale * std::cos(angle)) / xscale);
	return ret;
}

/**
 * Draw a game coordinate as a vertex.
 * @param coord The game coordinate to draw.
 */
void vertexPair(const Coordinate& coord) {
	pair<float, float> screenCoord = coordToScreen(coord);
	glVertex2f(screenCoord.first, screenCoord.second);
}

/**
 * Take a texture coordinate (x between 0 and 2048 and y between 0 and 1024, corresponding to the sprite sheet).
 */
void texCoordPair(const pair<float, float>& p) {
	glTexCoord2f(p.first / 2048., p.second / 1024.);
}

/**
 * The average point in a vector of points.
 * @param points The points to aggregate.
 * @return The average of the points.
 */
pair<float, float> averagePoint(const std::vector<pair<float, float>>& points) {
	pair<float, float> average;
	for(auto& it : points) {
		average.first += it.first;
		average.second += it.second;
	}
	average.first /= (float) points.size();
	average.second /= (float) points.size();
	return average;
}
