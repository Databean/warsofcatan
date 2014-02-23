#ifndef ROAD_H
#define ROAD_H

class Road public CornerPiece {
private:

public:
	Road();
	Road(Road&) = delete;
	~Road();
	Road& operator=(Road&) = delete;
};

#endif
