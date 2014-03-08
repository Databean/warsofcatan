#include "Road.h"

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

Coordinate Road::getStart() const{
	return start;
}
Coordinate Road::getEnd() const{
	return end;
}

bool Road::equals(const Road& otherRoad){
	Coordinate otherstart = otherRoad.getStart();
	Coordinate otherend = otherRoad.getEnd();
	return equals(otherstart, otherend);
}

bool Road::equals(const Coordinate& otherStart, const Coordinate& otherEnd){
	if((otherStart == start && otherEnd == end) || (otherStart == end && otherEnd == start))
		return true;
	return false;
}



bool Road::isMarked(){
	return marker;
}

void Road::mark(){
	marker = true;
}

void Road::unmark(){
	marker = false;
}



