#ifndef ROAD_H
#define ROAD_H

#include "Util.h"
#include "Player.h"
#include <vector>
#include <cmath>

class Road {
private:
	bool checkRoad();

public:
	Road(Coordinate start, Coordinate end, Player& Owner);
	Road(Road&) = delete;
	~Road();
	Road& operator=(Road&) = delete;

	//Made these public because I couldn't get getters to work, maybe someone else knows how. Paul
	Coordinate start;
	Coordinate end;

	bool equals(const Road& otherRoad);
	bool equals(const Coordinate& otherStart, const Coordinate& otherEnd);

	Player* owner;


};

#endif
