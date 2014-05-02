#ifndef ROAD_H
#define ROAD_H

#include "Util.h"
#include "Player.h"
#include <vector>
#include <cmath>
#include <stdexcept>

#include "Player.h"

class GameVisitor;

/**
 * Constructed between two points on the board. Settlements can only be constructed if a player has a road to a given tile.
 */
class Road {
private:
	Player& owner;
	Coordinate start;
	Coordinate end;

	bool checkRoad();
public:

	static bool isValidRoad(Coordinate start, Coordinate end);

	Road(Coordinate start, Coordinate end, Player& Owner);
	Road(Road&) = delete;
	virtual ~Road();
	Road& operator=(Road&) = delete;

	Coordinate getStart() const;
	Coordinate getEnd() const;

	bool operator==(const Road&) const;
	bool equals(const Road& otherRoad) const;
	bool equals(const Coordinate& otherStart, const Coordinate& otherEnd) const;

	Player& getOwner();

	virtual void accept(GameVisitor& visitor);
};

#endif
