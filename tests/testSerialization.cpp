#include <UnitTest++.h>

#include "XMLVisitor.h"
#include "GameBoard.h"
#include "Player.h"

#include "tinyxml2.h"

#include <iostream>

TEST(xmlPrint) {
	GameBoard testBoard;
	
	XMLVisitor vs;
	testBoard.accept(vs);
	
	tinyxml2::XMLPrinter printer;
	vs.getXMLDoc().Print( &printer );
	
	std::cout << printer.CStr() << std::endl;
}