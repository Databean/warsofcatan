#ifndef ROAD_H
#define ROAD_H

#include "Util.h"

#include "Player.h"

class GameVisitor;

class Road {
private:
	Player& owner;
	Coordinate start;
	Coordinate end;
public:
	Road(Player& owner, Coordinate start, Coordinate end);
	Road(Road&) = delete;
	~Road();
	Road& operator=(Road&) = delete;
	
	virtual void accept(GameVisitor& visitor);
	Coordinate getStart() const;
	Coordinate getEnd() const;
	Player& getOwner();
};

#endif
