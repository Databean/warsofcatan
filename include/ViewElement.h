#ifndef VIEW_ELEMENT_H
#define VIEW_ELEMENT_H

#include <vector>
#include <memory>
#include <array>

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "GL/gl.h"

#include "Util.h"


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
	std::string initiating;
	std::string receiving;

	ViewButtonText trade;
	ViewButtonText cancel;

	std::array<int, 5> offer;

	TradingView(TradingView& o) = delete;
	TradingView& operator=(TradingView& o) = delete;
protected:
	virtual bool clicked(ScreenCoordinate coord);
public:
	TradingView(const std::string& initiating, const std::string& receiving, std::function<bool(std::array<int, 5>, ScreenCoordinate)> trade, std::function<bool(ScreenCoordinate)> cancel, std::array<int, 5> offer);
	virtual ~TradingView();

	void render();
};


class ConfirmationDialogue : public ViewElement {
private:
	ScreenCoordinate topLeft;
	ScreenCoordinate bottomRight;

	std::string message;

	std::unique_ptr<ViewElement> confirmButton;
	std::unique_ptr<ViewElement> cancelButton;

protected:
	virtual bool clicked(ScreenCoordinate coord);

public:
	ConfirmationDialogue(std::function<bool(ScreenCoordinate)> confirm_action, std::function<bool(ScreenCoordinate)> cancel_action, std::pair<ScreenCoordinate, ScreenCoordinate> rect, std::string message);
	void render();
};

/**
 * Creates a unique_ptr to a confirmation dialogue view element.
 *
 * @param confirm_fn the function to run when clicking the confirm button
 * @param cancel_fn the function to run when clicking the cancel button
 * @param rect the coorinates of the bottom left and top right protions of the main dialogue box
 * @param message the message to display in the confirmation box
 * @return a unique_ptr to a newly created confirmation dialogue in the form of a view element
 */
template<class Fn>
std::unique_ptr<ViewElement> makeConfirmationDialogue(Fn confirm_fn, Fn cancel_fn, std::pair<ScreenCoordinate, ScreenCoordinate> rect, std::string message) {
	return std::unique_ptr<ViewElement>(new ConfirmationDialogue(confirm_fn, cancel_fn, rect, message));
}













#endif
