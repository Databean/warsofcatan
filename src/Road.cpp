#include "Road.h"

#include "GameVisitor.h"

Road::Road(Coordinate start, Coordinate end, Player& Owner) {
	if(start < end) {
		this->start = start;
		this->end = end;
	} else {
		this->end = start;
		this->start = end;
	}
	owner = &Owner;

	marker = false;

	//If the input is bad, throw an exception so bad roads won't be built
	if(!checkRoad()){
		throw -1;
	}


}

/**
 * Valid roads must start in one point and end in another point a distance of 1 away.
 */
bool Road::checkRoad(){
	if (start == end)
		return false;

	int dist = std::abs((start.first - end.first) + (start.second - end.second));
	return (dist <= 1);
}

Road::~Road() {
	
}

void Road::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

bool Road::operator==(const Road& other) const {
	return getStart() == other.getStart() &&
		getEnd() == other.getEnd() &&
		owner->getName() == other.owner->getName();
}

/**
 * Returns the starting coordinate of the road
 */
Coordinate Road::getStart() const{
	return start;
}

/**
 * Returns the ending coordinate of the road
 */
Coordinate Road::getEnd() const{
	return end;
}

/**
 * Returns true if the roads have the same coordinates whether they are matching starting or ending coordinates doesn't matter
 */
bool Road::equals(const Road& otherRoad){
	Coordinate otherstart = otherRoad.getStart();
	Coordinate otherend = otherRoad.getEnd();
	return equals(otherstart, otherend);
}

/**
 * Returns true if the roads have the same coordinates whether they are matching starting or ending coordinates doesn't matter
 */
bool Road::equals(const Coordinate& otherStart, const Coordinate& otherEnd){
	if((otherStart == start && otherEnd == end) || (otherStart == end && otherEnd == start))
		return true;
	return false;
}

/**
 * Returns true if the road is marked, false otherwise
 */
bool Road::isMarked(){
	return marker;
}

/**
 * Marks the road
 */
void Road::mark(){
	marker = true;
}

/**
 * Unmarks the road
 */
void Road::unmark(){
	marker = false;
}

Player& Road::getOwner() {
	return *owner;
}

Player& Road::getOwner() const {
	return *owner;
}
