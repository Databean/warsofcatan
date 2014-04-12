#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <vector>
#include <memory>
#include <array>

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
	
	std::map<int, std::unique_ptr<ViewElement>> viewElements;
	
	GameView(const GameView& o) = delete;
	GameView& operator=(const GameView& o) = delete;
public:
	GameView(GameBoard&);
	~GameView();
	
	void render();
	bool acceptInput(SDL_Event& event);
	
	void addElement(std::unique_ptr<ViewElement>);
	void addElement(int priority, std::unique_ptr<ViewElement>);
	
	std::unique_ptr<ViewElement> removeElement(int priority);
	std::unique_ptr<ViewElement> removeElement(const ViewElement*);
	std::unique_ptr<ViewElement> removeElement(const ViewElement&);
};

/**
 * A visitor of the GameBoard hierarchy that draws the entire model.
 */
class DrawingGameVisitor : public GameVisitor {
private:
	GameView& view;
	
	DrawingGameVisitor(const DrawingGameVisitor& o) = delete;
	DrawingGameVisitor& operator=(const DrawingGameVisitor& o) = delete;
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
class ViewButton : public ViewElement {
private:
	std::function<bool(ScreenCoordinate)> action;
	
	ViewButton(const ViewButton& vb) = delete;
	ViewButton& operator=(const ViewButton&) = delete;
protected:
	virtual bool clicked(ScreenCoordinate coord);
public:
	ViewButton(std::function<bool(ScreenCoordinate)> action, std::pair<ScreenCoordinate, ScreenCoordinate> rect);
	virtual ~ViewButton();
	
	virtual void render();
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
	return std::unique_ptr<ViewElement>(new ViewButton(fn, rect));
}

/**
 * A view element drawn as a solid color that has a callback function that is called when it is clicked.
 */
class ViewButtonColor : public ViewButton {
private:
	std::tuple<float, float, float> color;
	
	ViewButtonColor(const ViewButtonColor& vb) = delete;
	ViewButtonColor& operator=(const ViewButtonColor& vb) = delete;
public:
 	ViewButtonColor(std::function<bool(ScreenCoordinate)> action, std::pair<ScreenCoordinate, ScreenCoordinate> rect, std::tuple<float, float, float> color);
	virtual ~ViewButtonColor();
	
	virtual void render();
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
	return std::unique_ptr<ViewElement>(new ViewButtonColor(fn, rect, color));
}

/**
 * A view element drawn as some text on the screen that has a callback function when it is clicked.
 */
class ViewButtonText : public ViewButton {
private:
	GLuint texture;
	
	ViewButtonText(const ViewButtonText& vb) = delete;
	ViewButtonText& operator=(const ViewButtonText& vb) = delete;
public:
	ViewButtonText(std::function<bool(ScreenCoordinate)> action, std::pair<ScreenCoordinate, ScreenCoordinate> rect, const std::string& font, int fontSize, const std::string& text);
	virtual ~ViewButtonText();
	
	void setText(const std::string& font, int fontSize, const std::string& text);
	
	virtual void render();
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
	return std::unique_ptr<ViewElement>(new ViewButtonText(fn, rect, font, fontSize, text));
}

class TradingView : public ViewElement {
private:
	Player& initiating;
	Player& receiving;
	
	ViewButtonText trade;
	ViewButtonText cancel;
	
	std::array<int, 5> offer;
	
	TradingView(TradingView& o) = delete;
	TradingView& operator=(TradingView& o) = delete;
protected:
	virtual bool clicked(ScreenCoordinate coord);
public:
	TradingView(Player& initiating, Player& receiving, std::function<bool(std::array<int, 5>, ScreenCoordinate)> trade, std::function<bool(ScreenCoordinate)> cancel, std::array<int, 5> offer);
	virtual ~TradingView();
	
	void render();
};

#endif