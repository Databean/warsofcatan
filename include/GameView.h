#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "SDL2/SDL.h"

#include "GameVisitor.h"
#include "Util.h"

class GameBoard;
class GameController;

class GameView {
private:
	GameBoard& model;
	GameController& controller;
	
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

#endif