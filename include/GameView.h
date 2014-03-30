#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <vector>
#include <memory>

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "GL/gl.h"

#include "GameVisitor.h"
#include "Util.h"

class GameBoard;
class GameController;
class ViewElement;
class GameView;

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

class ClickCoordinateEvent {
private:
	Coordinate clicked;
public:
	ClickCoordinateEvent(const Coordinate& clicked);
	ClickCoordinateEvent(const ClickCoordinateEvent&);
	~ClickCoordinateEvent();
	ClickCoordinateEvent& operator=(const ClickCoordinateEvent&);
	
	Coordinate getCoordinate() const;
};

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

template<class Fn>
std::unique_ptr<ViewElement> makeViewButton(Fn fn, std::pair<ScreenCoordinate, ScreenCoordinate> rect) {
	return std::unique_ptr<ViewElement>(new ViewButton<Fn>(fn, rect));
}

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

template<class Fn>
std::unique_ptr<ViewElement> makeViewButtonColor(Fn fn, std::pair<ScreenCoordinate, ScreenCoordinate> rect, std::tuple<float, float, float> color) {
	return std::unique_ptr<ViewElement>(new ViewButtonColor<Fn>(fn, rect, color));
}

#endif