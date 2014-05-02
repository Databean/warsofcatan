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
 * The class in charge of drawing the view to the screen, using OpenGL calls.
 */
class GameView {
private:
	GameBoard& model;

	std::map<int, std::unique_ptr<ViewElement>> viewElements;
	std::vector<ScreenCoordinate> pointsOfInterest;

	void highlightPoint(ScreenCoordinate & coord);

	std::string controlStateText;

	GameView(const GameView& o) = delete;
	GameView& operator=(const GameView& o) = delete;

public:
	GameView(GameBoard&);
	~GameView();

	void render();
	bool acceptInput(SDL_Event& event);

	void setControlStateText(std::string newText);

	void addPointOfInterest(ScreenCoordinate);
	void clearPointsOfInterest();
	void addElement(std::unique_ptr<ViewElement> element);
	void addElement(int priority, std::unique_ptr<ViewElement>);

	std::unique_ptr<ViewElement> removeElement(int priority);
	std::unique_ptr<ViewElement> removeElement(const ViewElement*);
	std::unique_ptr<ViewElement> removeElement(const ViewElement&);

	void drawCardCount(std::string font, int fontSize);
	void drawResourceCount(std::string font, int fontSize);
	bool showTotals;
};

/**
 * A visitor of the GameBoard hierarchy that draws the entire model.
 */
class DrawingGameVisitor: public GameVisitor {
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
	virtual void visit(GameDice&);
	virtual void visit(Wonder&);
};

#endif
