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
	GameView& view;
	std::pair<ScreenCoordinate, ScreenCoordinate> rect;
	
	ViewElement(const ViewElement& vw) : view(vw.view) {} //deleted
	ViewElement& operator=(const ViewElement&) { return *this; } // deleted
protected:
	virtual bool clicked(ScreenCoordinate coord) = 0;
public:
	ViewElement(GameView& view, decltype(rect) rect);
	virtual ~ViewElement();
	
	virtual decltype(rect) getRect() const;
	virtual bool containsPoint(ScreenCoordinate coord) const;
	virtual bool handleClick(ScreenCoordinate coord);
	virtual void render() = 0;
};

class GameView {
private:
	GameBoard& model;
	GameController& controller;
	
	std::vector<std::unique_ptr<ViewElement>> viewElements;
	
	GameView(const GameView& o) : model(o.model), controller(o.controller) {} //deleted
	GameView& operator=(const GameView& o) { return *this; } //deleted
public:
	GameView(GameBoard&, GameController&);
	~GameView();
	
	void render();
	bool acceptInput(SDL_Event& event);
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
	std::tuple<float, float, float> color;
	
	ViewButton(const ViewButton& vb) : ViewElement(vb) {} //deleted
	ViewButton& operator=(const ViewButton& vb) { return *this; }
protected:
	virtual bool clicked(ScreenCoordinate coord) {
		return action(coord);
	}
public:
	ViewButton(Fn& action, std::pair<ScreenCoordinate, ScreenCoordinate> rect, std::tuple<float, float, float> color) : ViewElement(rect), action(action), color(color) {}
	~ViewButton() {}
	
	void render() {
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor3f(std::get<0>(color), std::get<1>(color), std::get<2>(color));
		auto topLeft = getRect().first;
		auto bottomRight = getRect().second;
		glBegin(GL_QUADS);
		glVertex3f(topLeft.first, topLeft.second);
		glVertex3f(bottomRight.first, topLeft.second);
		glVertex3f(bottomRight.first, bottomRight.second);
		glVertex3f(topLeft.first, bottomRight.second);
		glEnd();
	}
};

#endif