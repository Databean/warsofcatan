#ifndef GAME_VISITOR_H
#define GAME_VISITOR_H

class GameBoard;
class ResourceTile;
class Settlement;
class Road;
class City;
class Player;

class GameVisitor {
private:
	//Block copy constructor and assignment operator
	GameVisitor(GameVisitor&) {}
	GameVisitor& operator=(GameVisitor&) { return *this; }
public:
	GameVisitor();
	virtual ~GameVisitor();
	
	virtual void visit(GameBoard&) = 0;
	virtual void visit(Road&) = 0;
	virtual void visit(Settlement&) = 0;
	virtual void visit(City&) = 0;
	virtual void visit(Player&) = 0;
	virtual void visit(ResourceTile&) = 0;
};

#endif