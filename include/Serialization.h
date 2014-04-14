#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <map>
#include <string>
#include <set>

#include "GameVisitor.h"
#include "Util.h"

#include "tinyxml2.h"

/**
 * A visitor that populates a tinyxml2::XMLDocument with the state of a GameBoard model.
 */
class XMLVisitor : public GameVisitor {
private:
	tinyxml2::XMLDocument xmldoc;
	std::map<std::string, tinyxml2::XMLElement*> playerElementMap;
	std::set<Road*> serializedRoads;
	
	Player* lastPlayer;
	
	tinyxml2::XMLElement* coordinateElement(const Coordinate& c);
public:
	XMLVisitor();
	virtual ~XMLVisitor();
	
	virtual void visit(GameBoard&);
	virtual void visit(Road&);
	virtual void visit(Settlement&);
	virtual void visit(City&);
	virtual void visit(Player&);
	virtual void visit(ResourceTile&);
	virtual void visit(DevelopmentCard&);
	virtual void visit(GameDice&);
	virtual void visit(Wonder&);
	
	const tinyxml2::XMLDocument& getXMLDoc() const;
};

Coordinate xmlElementToCoord(const tinyxml2::XMLElement& element);

#endif
