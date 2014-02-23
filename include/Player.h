#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "Resources.h"

class Player {
private:
	std::string name;
	Resources resources;
public:
	Player(const std::string& name);
	Player(Player&) = delete;
	~Player();
	Player& operator=(Player&) = delete;
	
	const std::string& getName() const;
	void setName(const std::string& newName);
	
	const Resources& getResources() const;
	void setResources(const Resources& newResources);
};

#endif