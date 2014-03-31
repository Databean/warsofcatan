#ifndef ROAD_H
#define ROAD_H

#include "Util.h"
#include "Player.h"
#include <vector>
#include <cmath>
#include <stdexcept>

#include "Player.h"

class GameVisitor;

class Road {
private:
	Player& owner;
	Coordinate start;
	Coordinate end;
	
	bool checkRoad();
public:
	Road(Coordinate start, Coordinate end, Player& Owner);
	Road(Road&) = delete;
	virtual ~Road();
	Road& operator=(Road&) = delete;

	Coordinate getStart() const;
	Coordinate getEnd() const;

	bool equals(const Road& otherRoad);
	bool equals(const Coordinate& otherStart, const Coordinate& otherEnd);
	
	Player& getOwner();
	const Player& getOwner() const;
	
	virtual void accept(GameVisitor& visitor);
	bool operator==(const Road&) const;
};

#endif
