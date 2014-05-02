#include "ViewElement.h"

#include "Config.h"
#include "Renderer.h"

/**
 * Construct a ViewElement covering a particular rectangle on screen.
 * @param rect The rectangle on screen that the view element occupies.
 */
ViewElement::ViewElement(decltype(rect) rect) {
	using std::min;
	using std::max;

	//rect.first is the min x/min y corner
	//rect.second is the max x/max y corner
	this->rect.first = {min(rect.first.first, rect.second.first), min(rect.first.second, rect.second.second)};
	this->rect.second = {max(rect.first.first, rect.second.first), max(rect.first.second, rect.second.second)};
}

/**
 * Destroy the view element.
 */
ViewElement::~ViewElement() {

}

/**
 * The rectangle that the ViewElement occupies on screen.
 * @return The rectangle.
 */
decltype(ViewElement::rect) ViewElement::getRect() const {
	return rect;
}

/**
 * Determines if a ScreenCoordinate clicked on lies inside this ViewElement.
 * @return Whether the point is contained.
 */
bool ViewElement::containsPoint(ScreenCoordinate coord) const {
	return rect.first.first < coord.first && rect.first.second < coord.second
			&& coord.first < rect.second.first
			&& coord.second < rect.second.second;
}

/**
 * Use this ViewElement's click handler if the coordinate clicked on lies inside the rectangle.
 */
bool ViewElement::handleClick(ScreenCoordinate coord) {
	if (containsPoint(coord)) {
		return clicked(coord);
	}
	return false;
}

/**
 * Construct a ViewButton.
 * @param action The action to take when the button is pressed.
 * @param rect The area on the screen that the button receives clicks from.
 */
ViewButton::ViewButton(std::function<bool(ScreenCoordinate)> action,
		std::pair<ScreenCoordinate, ScreenCoordinate> rect) :
		ViewElement(rect), action(action) {

}

/**
 * Destroy the ViewButton.
 */
ViewButton::~ViewButton() {

}

/**
 * Render the ViewButton.
 */
void ViewButton::render() {

}

/**
 * Process a click that happens on this ViewButton.
 * @param coord The coordinate clicked on.
 * @return If the ViewButton handled the event.
 */
bool ViewButton::clicked(ScreenCoordinate coord) {
	return action(coord);
}

/**
 * Construct the ViewButtonColor.
 * @param action The action to take when the button is clicked.
 * @param rect The rectangle the button receives clicks from and is drawn in.
 * @param color The color to draw the button.
 */
ViewButtonColor::ViewButtonColor(std::function<bool(ScreenCoordinate)> action,
		std::pair<ScreenCoordinate, ScreenCoordinate> rect,
		std::tuple<float, float, float> color) :
		ViewButton(action, rect), color(color) {

}

/**
 * Destroy the ViewButtonColor.
 */
ViewButtonColor::~ViewButtonColor() {

}

/**
 * Render the ViewButtonColor as a solid rectangle in the rect given.
 */
void ViewButtonColor::render() {
	renderRectangle(getRect().first, getRect().second, color);
}

/**
 * Construct a ViewButtonText.
 * @param action The callback to invoke on clicking the button.
 * @param rect The rectangle to draw it to the screen to and accept inputs from.
 * @param font The path to the font of the text to render.
 * @param fontSize The font size to draw the text at.
 * @param text The text to show.
 */
ViewButtonText::ViewButtonText(std::function<bool(ScreenCoordinate)> action,
		std::pair<ScreenCoordinate, ScreenCoordinate> rect,
		const std::string& font, int fontSize, const std::string& text) :
		ViewButton(action, rect), texture(0) {

	setText(font, fontSize, text);
}

/**
 * Destroy the ViewButtonText.
 */
ViewButtonText::~ViewButtonText() {
	glDeleteTextures(1, &texture);
}

/**
 * Change the text displayed by the ViewButtonText.
 * @param font The path to the font to draw the text in.
 * @param fontSize The size of the font to draw the text as.
 * @param text The text to draw.
 */
void ViewButtonText::setText(const std::string& font, int fontSize,
		const std::string& text) {
	if (texture != 0) {
		glDeleteTextures(1, &texture);
	}
	texture = loadTextAsTexture(font, fontSize, text);
}

/**
 * Render the ViewButtonText with the given text.
 */
void ViewButtonText::render() {
	renderText(ViewElement::getRect().first, ViewElement::getRect().second,
			texture);
}

/**
 * Construct a trading view.
 * @param initiating The player initiating the trade.
 * @param receiving The other player involved in the trade.
 * @param trade The callback for the "trade" button, invoked with the current offer.
 * @param cancel The callback for the "cancel" button
 * @param initialOffer The initial offer to display.
 */
TradingView::TradingView(const std::string& initiating,
		const std::string& receiving,
		std::function<bool(std::array<int, 5>, ScreenCoordinate)> trade,
		std::function<bool(ScreenCoordinate)> cancel,
		std::array<int, 5> initialOffer) :
		ViewElement( { getGraphicsConfig()["screen.tradingView.bottomLeft"],
				getGraphicsConfig()["screen.tradingView.topRight"] }), initiating(
				initiating), receiving(receiving), trade(
				std::bind(trade, std::ref(offer), std::placeholders::_1),
				{
						getGraphicsConfig()["screen.tradingView.tradeButton.bottomLeft"],
						getGraphicsConfig()["screen.tradingView.tradeButton.topRight"] },
				getGraphicsConfig()["font.path"],
				getGraphicsConfig()["font.size"], "Trade"), cancel(cancel,
				{
						getGraphicsConfig()["screen.tradingView.cancelButton.bottomLeft"],
						getGraphicsConfig()["screen.tradingView.cancelButton.topRight"] },
				getGraphicsConfig()["font.path"],
				getGraphicsConfig()["font.size"], "Cancel"), offer(initialOffer) {
}

/**
 * Destroy the TradingView.
 */
TradingView::~TradingView() {

}

/**
 * Handle a user clicking on the TradingView.
 * @param coord The coordinate clicked on.
 */
bool TradingView::clicked(ScreenCoordinate coord) {
	if (cancel.handleClick(coord)) {
		return true;
	} else if (trade.handleClick(coord)) {
		return true;
	}
	int modifier = coord.first <= 0.5 ? -1 : 1;
	int resource = (coord.second
			- getGraphicsConfig()["screen.tradingView.resources.bottomY"])
			/ getGraphicsConfig()["screen.tradingView.resources.height"];
	if (resource >= 0 && resource <= 5) {
		offer[resource] += modifier;
	}
	return true;
}

/**
 * Render the TradingView.
 */
void TradingView::render() {
	glBindTexture(GL_TEXTURE_2D, 0);

	glColor3d(1, 1, 0);
	auto topLeft = ViewElement::getRect().first;
	auto bottomRight = ViewElement::getRect().second;
	renderRectangle(topLeft, bottomRight,
			std::tuple<float, float, float>(1, 1, 0));

	auto font = getGraphicsConfig()["font.path"];
	auto fontSize = getGraphicsConfig()["font.size"];

	std::string resources[] = { "Wheat", "Wool", "Ore", "Brick", "Wood" };
	for (int i = 0; i < 5; i++) {
		auto leftX = getGraphicsConfig()["screen.tradingView.resources.leftX"];
		auto rightX = getGraphicsConfig()["screen.tradingView.resources.rightX"];
		auto height = getGraphicsConfig()["screen.tradingView.resources.height"];
		auto bottomY =
				getGraphicsConfig()["screen.tradingView.resources.bottomY"];
		renderText(font, fontSize, { leftX, bottomY + (i * height) },
				{ rightX, bottomY + height + (i * height) },
				toString(offer[i]) + " " + resources[i]);
	}
	auto playersBottomLeft =
			getGraphicsConfig()["screen.tradingView.players.bottomLeft"];
	auto playersTopRight =
			getGraphicsConfig()["screen.tradingView.players.topRight"];
	renderText(font, fontSize, playersBottomLeft, playersTopRight,
			initiating + " -> " + receiving);

	cancel.render();
	trade.render();
}

/**
 * Constructor of the confirmation dialogue. A confirmation dialogue is a window that contiains a "Yes" and "No" option. Each of which can have an
 * action bound to them. The idea is to use this dialogue to give the user control on an action on whather or not he wants to go through with it.
 * @param confirm_action the function to run when "yes" is pressed
 * @param cancel_action the function to run when "no" is pressed
 * @param rect the Coordinates of the bottom left and top right portions of the box.
 * @param message the message to display in the dialogue
 */
ConfirmationDialogue::ConfirmationDialogue(
		std::function<bool(ScreenCoordinate)> confirm_action,
		std::function<bool(ScreenCoordinate)> cancel_action,
		std::pair<ScreenCoordinate, ScreenCoordinate> rect, std::string message) :
		ViewElement(rect), message(message) {
	topLeft = ViewElement::getRect().first;
	bottomRight = ViewElement::getRect().second;

	float width = bottomRight.first - topLeft.first;
	float height = bottomRight.second - topLeft.second;
	ScreenCoordinate confirmTopLeft = ScreenCoordinate(
			topLeft.first + (width * .1), topLeft.second + (height * .1));
	ScreenCoordinate confirmBottomRight = ScreenCoordinate(
			bottomRight.first - (width * .6),
			bottomRight.second - (height * .6));
	ScreenCoordinate cancelTopLeft = ScreenCoordinate(
			topLeft.first + (width * .6), topLeft.second + (height * .1));
	ScreenCoordinate cancelBottomRight = ScreenCoordinate(
			bottomRight.first - (width * .1),
			bottomRight.second - (height * .6));

	auto font = getGraphicsConfig()["font.path"];
	auto fontSize = getGraphicsConfig()["font.size"];

	confirmButton = std::unique_ptr<ViewElement>(
			new ViewButtonText(confirm_action, { confirmTopLeft,
					confirmBottomRight }, font, fontSize, "Yes"));
	cancelButton = std::unique_ptr<ViewElement>(
			new ViewButtonText(cancel_action,
					{ cancelTopLeft, cancelBottomRight }, font, fontSize,
					"No"));
}

/**
 * Checks to see if either the yes or no option was clicked and runs the functions associated with them if they were.
 * @param coord the cooridinate clicked on the screen.
 */
bool ConfirmationDialogue::clicked(ScreenCoordinate coord) {
	if (confirmButton->handleClick(coord)) {
		return true;
	} else if (cancelButton->handleClick(coord)) {
		return true;
	}
	return false;
}

/**
 * Renders the object on the screen.
 */
void ConfirmationDialogue::render() {
	renderRectangle(topLeft, bottomRight,
			std::tuple<float, float, float>(1., 1., 1.));

	auto font = getGraphicsConfig()["font.path"];
	auto fontSize = getGraphicsConfig()["font.size"];

	float width = bottomRight.first - topLeft.first;
	float height = bottomRight.second - topLeft.second;

	renderText(font, fontSize,
			{ topLeft.first + .05 * width, topLeft.second + .4 * height },
			{ bottomRight.first - .05 * width, bottomRight.second - .15 * height },
			message);
	confirmButton->render();
	cancelButton->render();
}

