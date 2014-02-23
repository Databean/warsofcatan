#ifndef RESOURCES_H
#define RESOURCES_H

class Resources {
public:
	typedef unsigned short rtype;
private:
	rtype wood;
	rtype sheep;
	rtype grain;
	rtype ore;
	rtype brick;
public:
	Resources();
	Resources(rtype wood, rtype sheep, rtype grain, rtype ore, rtype brick);
	Resources(const Resources&);
	~Resources();
	Resources& operator=(const Resources&);
	
	rtype getWood() const;
	void setWood(rtype newWood);
	
	rtype getSheep() const;
	void setSheep(rtype newSheep);
	
	rtype getGrain() const;
	void setGrain(rtype newGrain);
	
	rtype getOre() const;
	void setOre(rtype newOre);
	
	rtype getBrick() const;
	void setBrick(rtype newBrick);
	
	Resources operator+(const Resources&) const;
	Resources& operator+=(const Resources&);
	
	Resources operator-(const Resources&) const;
	Resources& operator-=(const Resources&);
	
	bool operator<(const Resources&) const;
	bool operator>(const Resources&) const;
};

#endif