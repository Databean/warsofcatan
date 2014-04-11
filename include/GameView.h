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
	std::vector<ScreenCoordinate> pointsOfInterest;

	void highlightPoint(ScreenCoordinate & coord);
	
	GameView(const GameView& o) : model(o.model) {} //deleted
	GameView& operator=(const GameView& o) { return *this; } //deleted
public:
	GameView(GameBoard&);
	~GameView();
	
	void render();
	bool acceptInput(SDL_Event& event);
	
	void addElement(std::unique_ptr<ViewElement>);
	bool removeElement(ViewElement*);
	bool removeLastElement();

	void addPointOfInterest(ScreenCoordinate);
	void clearPointsOfInterest();
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

template<class Fn>
class ConfirmationDialogue : public ViewElement {
private:
	Fn confirm_action;
	Fn cancel_action;

	bool clickedConfirm(ScreenCoordinate coord);
	bool clickedCancel(ScreenCoordinate coord);
	ScreenCoordinate topLeft;
	ScreenCoordinate bottomRight;
	ScreenCoordinate confirmTopLeft;
	ScreenCoordinate confirmBottomRight;
	ScreenCoordinate cancelTopLeft;
	ScreenCoordinate cancelBottomRight;


protected:
	virtual bool clicked_confirm(ScreenCoordinate coord){
		return confirmTopLeft.first < coord.first &&
				confirmTopLeft.second < coord.second &&
				coord.first < confirmBottomRight.first &&
				coord.second < confirmBottomRight.second;
	}
	virtual bool clicked_cancel(ScreenCoordinate coord){
			return cancelTopLeft.first < coord.first &&
					cancelTopLeft.second < coord.second &&
					coord.first < cancelBottomRight.first &&
					coord.second < cancelBottomRight.second;
	}

	virtual bool clicked(ScreenCoordinate coord){
		if(clicked_confirm(coord)){
			confirm_action(coord);
		} else if(clicked_cancel(coord)){
			cancel_action(coord);
		}
		return false;
	}

public:
	ConfirmationDialogue(Fn confirm_action, Fn cancel_action, std::pair<ScreenCoordinate, ScreenCoordinate> rect): ViewElement(rect), confirm_action(confirm_action), cancel_action(cancel_action){
		topLeft = ViewElement::getRect().first;
		bottomRight = ViewElement::getRect().second;

		float width = bottomRight.first - topLeft.first;
		float height = bottomRight.second - topLeft.second;
		confirmTopLeft = ScreenCoordinate(topLeft.first +(width*.1), topLeft.second+(height*.1));
		confirmBottomRight = ScreenCoordinate(bottomRight.first - (width * .6), bottomRight.second - (height * .6));
		cancelTopLeft = ScreenCoordinate(topLeft.first + (width*.6), topLeft.second + (height *.1));
		cancelBottomRight = ScreenCoordinate(bottomRight.first - (width * .1), bottomRight.second - (height * .6));

	}
	virtual void render(){
		glBindTexture(GL_TEXTURE_2D, 0);

		glColor3f(1., 1., 1.);
		glBegin(GL_QUADS);
		glVertex2f(topLeft.first, topLeft.second);
		glVertex2f(bottomRight.first, topLeft.second);
		glVertex2f(bottomRight.first, bottomRight.second);
		glVertex2f(topLeft.first, bottomRight.second);
		glEnd();

		glColor3f(0., 1., 0.);
		glBegin(GL_QUADS);
		glVertex2f(confirmTopLeft.first, confirmTopLeft.second);
		glVertex2f(confirmBottomRight.first, confirmTopLeft.second);
		glVertex2f(confirmBottomRight.first, confirmBottomRight.second);
		glVertex2f(confirmTopLeft.first, confirmBottomRight.second);
		glEnd();

		glColor3f(1.,0.,0.);
		glBegin(GL_QUADS);
		glVertex2f(cancelTopLeft.first, cancelTopLeft.second);
		glVertex2f(cancelBottomRight.first, cancelTopLeft.second);
		glVertex2f(cancelBottomRight.first, cancelBottomRight.second);
		glVertex2f(cancelTopLeft.first, cancelBottomRight.second);
		glEnd();
	}
};

template<class Fn>
std::unique_ptr<ViewElement> makeConfirmationDialogue(Fn confirm_fn, Fn cancel_fn, std::pair<ScreenCoordinate, ScreenCoordinate> rect) {
	return std::unique_ptr<ViewElement>(new ConfirmationDialogue<Fn>(confirm_fn, cancel_fn, rect));
}



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

#endif
