#ifndef CITY_H
#define CITY_H

class City public CornerPiece {
private:

public:
	City();
	City(City&) = delete;
	~City();
	City& operator=(City&) = delete;
};

#endif
