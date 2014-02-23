#include "Player.h"

using std::string;

Player::Player(const string& name) : name(name), resources() {
	
}

Player::~Player() {
	
}

const string& Player::getName() const {
	return name;
}

void Player::setName(const string& newName) {
	name = newName;
}

const Resources& Player::getResources() const {
	return resources;
}

void Player::setResources(const Resources& newResources) {
	resources = newResources;
}
