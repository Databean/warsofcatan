#ifndef XML_VISITOR_H
#define XML_VISITOR_H

#include "GameVisitor.h"

#include "tinyxml2.h"

class XMLVisitor : public GameVisitor {
private:
	tinyxml2::XMLDocument xmldoc;
public:
	XMLVisitor();
	virtual ~XMLVisitor();
	
	virtual void visit(GameBoard&);
	virtual void visit(Road&);
	virtual void visit(Settlement&);
	virtual void visit(City&);
	virtual void visit(Player&);
};

#endif