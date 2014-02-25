#include "Road.h"

Road::Road(Coordinate start, Coordinate end) {
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