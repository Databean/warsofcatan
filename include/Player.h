#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
	
public:
	Player();
	Player(Player&) = delete;
	~Player();
	Player& operator=(Player&) = delete;
};

#endif