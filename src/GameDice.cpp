#include "GameDice.h"
#include "GameVisitor.h"

int GameDice::getFirst(){
	return first;
}

int GameDice::getSecond(){
	return second;
}

void GameDice::setFirst(int newFirst){
	first = newFirst;
}

void GameDice::setSecond(int newSecond){
	second = newSecond;
}

void GameDice::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}