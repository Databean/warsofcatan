#include "XMLVisitor.h"

#include <map>
#include <utility>
#include <stdexcept>

#include "GameBoard.h"
#include "GamePiece.h"
#include "City.h"
#include "Settlement.h"
#include "Road.h"

using std::map;
using std::runtime_error;

using tinyxml2::XMLElement;
using tinyxml2::XMLText;

XMLVisitor::XMLVisitor() {
	xmldoc.InsertEndChild(xmldoc.NewElement("catangame"));
	//xmldoc.RootElement()->SetName("catangame");
}

XMLVisitor::~XMLVisitor() {
	
}

void XMLVisitor::visit(GameBoard& board) {
	
}

void XMLVisitor::visit(Road& road) {
	if(!xmldoc.RootElement()->FirstChildElement("roads")) {
		xmldoc.RootElement()->InsertEndChild(xmldoc.NewElement("roads"));
	}
	XMLElement* roadsElement = xmldoc.RootElement()->FirstChildElement("roads");
	XMLElement* newRoadElement = xmldoc.NewElement("road");
	
	XMLElement* ownerElement = xmldoc.NewElement("owner");
	XMLText* ownerText = xmldoc.NewText(road.getOwner().getName().c_str());
	ownerElement->InsertEndChild(ownerText);
	newRoadElement->InsertEndChild(ownerElement);
	
	XMLElement* startElement = xmldoc.NewElement("start");
	startElement->InsertEndChild(coordinateElement(road.getStart()));
	newRoadElement->InsertEndChild(startElement);
	
	XMLElement* endElement = xmldoc.NewElement("end");
	endElement->InsertEndChild(coordinateElement(road.getEnd()));
	newRoadElement->InsertEndChild(endElement);
	
	roadsElement->InsertEndChild(newRoadElement);
}

void XMLVisitor::visit(Settlement& settlement) {
	
}

void XMLVisitor::visit(City& city) {
	
}

void XMLVisitor::visit(Player& player) {
	
}

void XMLVisitor::visit(ResourceTile& tile) {
	if(!xmldoc.RootElement()->FirstChildElement("tiles")) {
		xmldoc.RootElement()->InsertEndChild(xmldoc.NewElement("tiles"));
	}
	XMLElement* tilesElement = xmldoc.RootElement()->FirstChildElement("tiles");
	XMLElement* newTileElement = xmldoc.NewElement("tile");
	
	static const map<resourceType, std::string> typeToText = {
		std::make_pair(WHEAT, "wheat"),
		std::make_pair(SHEEP, "sheep"),
		std::make_pair(STONE, "stone"),
		std::make_pair(BRICK, "brick"),
		std::make_pair(WOOD, "wood"),
		std::make_pair(DESERT, "desert"),
	};
	auto it = typeToText.find(tile.getType());
	if(it == typeToText.end()) {
		throw runtime_error("Invalid resource type.");
	}
	XMLElement* tileTypeElement = xmldoc.NewElement("type");
	tileTypeElement->InsertEndChild(xmldoc.NewText(it->second.c_str()));
	newTileElement->InsertEndChild(tileTypeElement);
	
	newTileElement->InsertEndChild(coordinateElement(tile.getCoordinates()));
	
	tilesElement->InsertEndChild(newTileElement);
}

XMLElement* XMLVisitor::coordinateElement(const Coordinate& c) {
	XMLElement* ret = xmldoc.NewElement("coordinate");
	ret->SetAttribute("u", c.first);
	ret->SetAttribute("v", c.second);
	return ret;
}

const tinyxml2::XMLDocument& XMLVisitor::getXMLDoc() const {
	return xmldoc;
}

