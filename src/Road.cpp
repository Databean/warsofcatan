#include "Road.h"

#include "GameVisitor.h"

Road::Road(Player& owner, Coordinate start, Coordinate end) : owner(owner) {
	if(start < end) {
		this->start = start;
		this->end = end;
	} else {
		this->end = start;
		this->start = end;
	}
}

Road::~Road() {
	
}

void Road::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

Coordinate Road::getStart() const {
	return start;
}

Coordinate Road::getEnd() const {
	return end;
}

Player& Road::getOwner() {
	return owner;
}

const Player& Road::getOwner() const {
	return owner;
}

bool Road::operator==(const Road& other) const {
	return getStart() == other.getStart() &&
		getEnd() == other.getEnd() &&
		getOwner() == other.getOwner();
}
