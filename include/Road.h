#ifndef ROAD_H
#define ROAD_H

#include "Util.h"

class Road {
private:
	Coordinate start;
	Coordinate end;
public:
	Road(Coordinate start, Coordinate end);
	Road(Road&) = delete;
	~Road();
	Road& operator=(Road&) = delete;
};

#endif
