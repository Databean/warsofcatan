#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <vector>
#include <memory>

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "GL/gl.h"

#include "GameVisitor.h"
#include "Renderer.h"
#include "Util.h"

class GameBoard;
class GameController;
class ViewElement;
class GameView;

/**
 * An element that is drawn on screen and can receive inputs from the user. These all occupy a rectangular area on screen
 * and can choose to handle clicks from the user that are inside their area.
 */
class ViewElement {
private:
	std::pair<ScreenCoordinate, ScreenCoordinate> rect;
	
	ViewElement(const ViewElement& vw) {} //deleted
	ViewElement& operator=(const ViewElement&) { return *this; } // deleted
protected:
	virtual bool clicked(ScreenCoordinate coord) = 0;
public:
	ViewElement(decltype(rect) rect);
	virtual ~ViewElement();
	
	virtual decltype(rect) getRect() const;
	virtual bool containsPoint(ScreenCoordinate coord) const;
	virtual bool handleClick(ScreenCoordinate coord);
	virtual void render() = 0;
};

/**
 * The class in charge of drawing the view to the screen, using OpenGL calls.
 */
class GameView {
private:
	GameBoard& model;
	
	std::vector<std::unique_ptr<ViewElement>> viewElements;
	
	GameView(const GameView& o) : model(o.model) {} //deleted
	GameView& operator=(const GameView& o) { return *this; } //deleted
public:
	GameView(GameBoard&);
	~GameView();
	
	void render();
	bool acceptInput(SDL_Event& event);
	
	void addElement(std::unique_ptr<ViewElement>);
};

/**
 * A visitor of the GameBoard hierarchy that draws the entire model.
 */
class DrawingGameVisitor : public GameVisitor {
private:
	GameView& view;
	
	DrawingGameVisitor(const DrawingGameVisitor& o) : view(o.view) {} //deleted
	DrawingGameVisitor& operator=(const DrawingGameVisitor& o) { return *this; } //deleted
public:
	DrawingGameVisitor(GameView& view);
	~DrawingGameVisitor();
	
	virtual void visit(GameBoard&);
	virtual void visit(Road&);
	virtual void visit(Settlement&);
	virtual void visit(City&);
	virtual void visit(Player&);
	virtual void visit(ResourceTile&);
	virtual void visit(DevelopmentCard&);
};

/**
 * A view element that is invisible and calls a callback function when it is clicked.
 */
template<class Fn>
class ViewButton : public ViewElement {
private:
	Fn action;
	
	ViewButton(const ViewButton& vb) : ViewElement(vb) {} //deleted
	ViewButton& operator=(const ViewButton&) { return *this; } //deleted
protected:
	virtual bool clicked(ScreenCoordinate coord) {
		return action(coord);
	}
public:
	ViewButton(Fn action, std::pair<ScreenCoordinate, ScreenCoordinate> rect) : ViewElement(rect), action(action) {}
	virtual ~ViewButton() {}
	
	virtual void render() {}
};

/**
 * Constructs a ViewButton using the same parameters as the ViewButton. Exists because template inference exists only
 * for functions, not classes.
 * @param fn The callback function to be called with the ScreenCoordinate clicked and returning a boolean on if it was handled.
 * @param rect The location on screen that the invisible button receives clicks from.
 * @return An owning unique pointer to the constructed view button.
 */
template<class Fn>
std::unique_ptr<ViewElement> makeViewButton(Fn fn, std::pair<ScreenCoordinate, ScreenCoordinate> rect) {
	return std::unique_ptr<ViewElement>(new ViewButton<Fn>(fn, rect));
}

/**
 * A view element drawn as a solid color that has a callback function that is called when it is clicked.
 */
template<class Fn>
class ViewButtonColor : public ViewButton<Fn> {
private:
	std::tuple<float, float, float> color;
	
	ViewButtonColor(const ViewButtonColor& vb) : ViewElement(vb) {} //deleted
	ViewButtonColor& operator=(const ViewButtonColor& vb) { return *this; }
public:
	ViewButtonColor(Fn action, std::pair<ScreenCoordinate, ScreenCoordinate> rect, std::tuple<float, float, float> color) : ViewButton<Fn>(action, rect), color(color) {}
	virtual ~ViewButtonColor() {}
	
	virtual void render() {
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor3f(std::get<0>(color), std::get<1>(color), std::get<2>(color));
		auto topLeft = ViewElement::getRect().first;
		auto bottomRight = ViewElement::getRect().second;
		glBegin(GL_QUADS);
		glVertex2f(topLeft.first, topLeft.second);
		glVertex2f(bottomRight.first, topLeft.second);
		glVertex2f(bottomRight.first, bottomRight.second);
		glVertex2f(topLeft.first, bottomRight.second);
		glEnd();
	}
};

/**
 * Constructs a ViewButtonColor using the same parameters as the ViewButtonColor. Exists because template inference exists only
 * for functions, not classes.
 * @param fn The callback function to be called with the ScreenCoordinate clicked and returning a boolean on if it was handled.
 * @param rect The location on screen to draw to and receive clicks from.
 * @param color The color to draw the button. RGB floats from 0 to 1 for intensity.
 * @return An owning unique pointer to the constructed view button.
 */
template<class Fn>
std::unique_ptr<ViewElement> makeViewButtonColor(Fn fn, std::pair<ScreenCoordinate, ScreenCoordinate> rect, std::tuple<float, float, float> color) {
	return std::unique_ptr<ViewElement>(new ViewButtonColor<Fn>(fn, rect, color));
}

/**
 * A view element drawn as some text on the screen that has a callback function when it is clicked.
 */
template<class Fn>
class ViewButtonText : public ViewButton<Fn> {
private:
	GLuint texture;
	
	ViewButtonText(const ViewButtonText& vb) : ViewElement(vb) {} //deleted
	ViewButtonText& operator=(const ViewButtonText& vb) { return *this; }
public:
	ViewButtonText(Fn action, std::pair<ScreenCoordinate, ScreenCoordinate> rect, const std::string& font, int fontSize, const std::string& text) : ViewButton<Fn>(action, rect) {
		setText(font, fontSize, text);
	}
	virtual ~ViewButtonText() {
		glDeleteTextures(1, &texture);
	}
	
	void setText(const std::string& font, int fontSize, const std::string& text) {
		if(texture != 0) {
			glDeleteTextures(1, &texture);
		}
		texture = loadTextAsTexture(font, fontSize, text);
	}
	
	virtual void render() {
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(1.0, 1.0, 1.0);
		auto topLeft = ViewElement::getRect().first;
		auto bottomRight = ViewElement::getRect().second;
		glBegin(GL_QUADS);
		glTexCoord2i(0, 1);
		glVertex2f(topLeft.first, topLeft.second);
		glTexCoord2i(1, 1);
		glVertex2f(bottomRight.first, topLeft.second);
		glTexCoord2i(1, 0);
		glVertex2f(bottomRight.first, bottomRight.second);
		glTexCoord2i(0, 0);
		glVertex2f(topLeft.first, bottomRight.second);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

/**
 * Constructs a ViewButtonText using the same parameters as the ViewButtonText. Exists because template inference exists only
 * for functions, not classes.
 * @param fn The callback function to be called with the ScreenCoordinate clicked and returning a boolean on if it was handled.
 * @param rect The location on screen to draw to and receive clicks from.
 * @param font The path to the font to use to draw the text.
 * @param fontSize The font size of the text.
 * @param text The text to render.
 */
template<class Fn>
std::unique_ptr<ViewElement> makeViewButtonText(Fn fn, std::pair<ScreenCoordinate, ScreenCoordinate> rect, const std::string& font, int fontSize, const std::string& text) {
	return std::unique_ptr<ViewElement>(new ViewButtonText<Fn>(fn, rect, font, fontSize, text));
}

#endif