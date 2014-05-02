#include "Road.h"

#include "GameVisitor.h"

/**
 * Construct a road.
 * @param start One endpoint of the road.
 * @param end The other endpoint of the road.
 * @param Owner The player owning the road.
 */
Road::Road(Coordinate start, Coordinate end, Player& Owner) :
		owner(Owner) {
	if (start < end) {
		this->start = start;
		this->end = end;
	} else {
		this->end = start;
		this->start = end;
	}

	//If the input is bad, throw an exception so bad roads won't be built
	if (!isValidRoad(start, end)) {
		throw std::invalid_argument(
				"Road must start on one corner and go to a corner exactly 1 away");
	}
}

/**
 * Destructor
 */
Road::~Road() {

}

/**
 * Valid roads must start in one point and end in another point a distance of 1 away.
 * @param start one end of the road
 * @param end the other end of the road
 * @return If the road is a valid road.
 */
bool Road::isValidRoad(Coordinate start, Coordinate end) {
	if (start == end)
		return false;
	if (abs((start.second - end.second) > 1))
		return false;

	int dist = std::abs(
			(start.first - end.first) + (start.second - end.second));
	return (dist <= 1);
}

/**
 * Accepts a GameVisitor. This is used to serialize a road object or render it.
 * @param visitor A GameVisitor that is visiting this road
 */
void Road::accept(GameVisitor& visitor) {
	visitor.visit(*this);
}

/**
 * The starting coordinate of the road.
 * @return The start.
 */
Coordinate Road::getStart() const {
	return start;
}

/**
 * The ending coordinate of the road.
 * @return The end.
 */
Coordinate Road::getEnd() const {
	return end;
}

/**
 * Equals operator overloaded for roads. Order of start and end doesn't matter as long as they are the same
 * Players also need to have the same names.
 * @param other the other road that this road is being compared to
 * @return true if the roads are equal, false otherwise
 */
bool Road::operator==(const Road& other) const {
	return equals(other);
}

/**
 * Compares this road with another road for equality.
 * @param otherRoad The road to compare this road two.
 * @return true if the roads have the same coordinates whether they are matching starting or ending coordinates doesn't matter
 */
bool Road::equals(const Road& otherRoad) const {
	Coordinate otherstart = otherRoad.getStart();
	Coordinate otherend = otherRoad.getEnd();
	return equals(otherstart, otherend)
			&& (owner.getName() == otherRoad.owner.getName());
}

/**
 * Compares this road with a pair of points for equality.
 * @param otherStart One endpoint of the road to compare to.
 * @param otherEnd The other endpoint of the road to compare to.
 * @return true if the roads have the same coordinates whether they are matching starting or ending coordinates doesn't matter
 */
bool Road::equals(const Coordinate& otherStart,
		const Coordinate& otherEnd) const {
	if ((otherStart == start && otherEnd == end)
			|| (otherStart == end && otherEnd == start))
		return true;
	return false;
}

/**
 * The owner of this road.
 * @return The owner.
 */
Player& Road::getOwner() {
	return owner;
}
