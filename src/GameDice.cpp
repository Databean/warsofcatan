#include "GameDice.h"
#include "GameVisitor.h"

#include <cstdlib>

using std::rand;

GameDice::GameDice() {
	first = 3;
	second = 3;
}

/**
 * Re-roll the dice.
 * @return The sum of the two dice.
 */
int GameDice::roll() {
	first = (rand() % 6) + 1;
	second = (rand() % 6) + 1;
	
	return first + second;
}

/**
 * The value of the first die.
 * @return The first die's value.
 */
int GameDice::getFirst() const {
	return first;
}

/**
 * The value of the second die.
 * @return The second die's value.
 */
int GameDice::getSecond() const {
	return second;
}

/**
 * Double-dispatch method for GameVisitor.
 * @param visitor The visitor.
 */
void GameDice::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}