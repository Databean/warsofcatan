#ifndef GAME_VISITOR_H
#define GAME_VISITOR_H

class GameBoard;
class ResourceTile;
class Settlement;
class Road;
class City;
class Player;
class DevelopmentCard;
class GameDice;
class Wonder;

/**
 * A class to be extended with callbacks to handle the different classes in the model.
 * Specific subclasses of this visitor pattern are used in Serialization and GameView
 */
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
	virtual void visit(DevelopmentCard&) = 0;
	virtual void visit(GameDice&) = 0;
	virtual void visit(Wonder&) = 0;
};

#endif
