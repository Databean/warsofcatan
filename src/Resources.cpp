#include "Resources.h"

#include <stdexcept>

using std::runtime_error;

Resources::Resources() : wood(0), sheep(0), grain(0), ore(0), brick(0) {
	
}

Resources::Resources(rtype wood, rtype sheep, rtype grain, rtype ore, rtype brick) :
	wood(wood), sheep(sheep), grain(grain), ore(ore), brick(brick) {
	
}

Resources::Resources(const Resources& other) {
	*this = other;
}

Resources::~Resources() {
	
}

Resources& Resources::operator=(const Resources& other) {
	wood = other.getWood();
	sheep = other.getSheep();
	grain = other.getGrain();
	ore = other.getOre();
	brick = other.getBrick();
	return *this;
}

Resources::rtype Resources::getWood() const {
	return wood;
}

void Resources::setWood(rtype newWood) {
	wood = newWood;
}

Resources::rtype Resources::getSheep() const {
	return sheep;
}

void Resources::setSheep(rtype newSheep) {
	sheep = newSheep;
}

Resources::rtype Resources::getGrain() const {
	return grain;
}

void Resources::setGrain(rtype newGrain) {
	grain = newGrain;
}

Resources::rtype Resources::getOre() const {
	return ore;
}

void Resources::setOre(rtype newOre) {
	ore = newOre;
}

Resources::rtype Resources::getBrick() const {
	return brick;
}

void Resources::setBrick(rtype newBrick) {
	brick = newBrick;
}

/**
 * Makes a new resource object by adding together the components of the two added objects.
 */
Resources Resources::operator+(const Resources& other) const {
	return Resources(wood + other.getWood(),
		sheep + other.getSheep(),
		grain + other.getGrain(),
		ore + other.getOre(),
		brick + other.getBrick());
}

/**
 * Adds the components of the passed in resource object to this object;
 */
Resources& Resources::operator+=(const Resources& other) {
	*this = *this + other;
	return *this;
}

/**
 * Subtracts the other resources from this one. Throws an exception if that
 * would cause the resources in the resulting object to be negative.
 */
Resources Resources::operator-(const Resources& other) const {
	if(*this < other) {
		throw std::runtime_error("Insufficient resources to subtract.");
	}
	return Resources(wood - other.getWood(),
		sheep - other.getSheep(),
		grain - other.getGrain(),
		ore - other.getOre(),
		brick - other.getBrick());
}

/**
 * Subtracts the components of the passed in resource object to this object.
 * Throws an exception if that would cause this object to have negative components.
 */
Resources& Resources::operator-=(const Resources& other) {
	*this = *this - other;
	return *this;
}

/**
 * True if any of the individual resources is less than the corresponding resource in the
 * other object. Useful for testing if a resource cost can be paid.
 * For example, if playerResources < cost, then the item cannot be bought;
 */
bool Resources::operator<(const Resources& other) const {
	return wood < other.getWood() ||
		sheep < other.getSheep() ||
		grain < other.getGrain() ||
		sheep < other.getSheep() ||
		ore < other.getOre() ||
		brick < other.getBrick();
}

/**
 * True if all of the individual resources are greater than 
 */
bool Resources::operator>(const Resources& other) const {
	return other < *this;
}

