#include "Serialization.h"

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
	if(!xmldoc.RootElement()->FirstChildElement("settlements")) {
		xmldoc.RootElement()->InsertEndChild(xmldoc.NewElement("settlements"));
	}
	XMLElement* settlementsElement = xmldoc.RootElement()->FirstChildElement("settlements");
	XMLElement* newSettlementElement = xmldoc.NewElement("settlement");
	
	XMLElement* ownerElement = xmldoc.NewElement("owner");
	ownerElement->InsertEndChild(xmldoc.NewText(settlement.getOwner().getName().c_str()));
	newSettlementElement->InsertEndChild(ownerElement);
	
	newSettlementElement->InsertEndChild(coordinateElement(settlement.getLocation()));
	
	settlementsElement->InsertEndChild(newSettlementElement);
}

void XMLVisitor::visit(City& city) {
	if(!xmldoc.RootElement()->FirstChildElement("cities")) {
		xmldoc.RootElement()->InsertEndChild(xmldoc.NewElement("cities"));
	}
	XMLElement* citiesElement = xmldoc.RootElement()->FirstChildElement("cities");
	XMLElement* newCityElement = xmldoc.NewElement("city");
	
	XMLElement* ownerElement = xmldoc.NewElement("owner");
	ownerElement->InsertEndChild(xmldoc.NewText(city.getOwner().getName().c_str()));
	newCityElement->InsertEndChild(ownerElement);
	
	newCityElement->InsertEndChild(coordinateElement(city.getLocation()));
	
	citiesElement->InsertEndChild(newCityElement);
}

void XMLVisitor::visit(Player& player) {
	if(!xmldoc.RootElement()->FirstChildElement("players")) {
		xmldoc.RootElement()->InsertEndChild(xmldoc.NewElement("players"));
	}
	XMLElement* playersElement = xmldoc.RootElement()->FirstChildElement("players");
	XMLElement* newPlayerElement = xmldoc.NewElement("player");
	
	XMLElement* nameElement = xmldoc.NewElement("name");
	nameElement->InsertEndChild(xmldoc.NewText(player.getName().c_str()));
	newPlayerElement->InsertEndChild(nameElement);
	
	XMLElement* woodElement = xmldoc.NewElement("wood");
	woodElement->InsertEndChild(xmldoc.NewText(toString(player.getWood()).c_str()));
	newPlayerElement->InsertEndChild(woodElement);
	
	XMLElement* brickElement = xmldoc.NewElement("brick");
	brickElement->InsertEndChild(xmldoc.NewText(toString(player.getBrick()).c_str()));
	newPlayerElement->InsertEndChild(brickElement);
	
	XMLElement* oreElement = xmldoc.NewElement("ore");
	oreElement->InsertEndChild(xmldoc.NewText(toString(player.getOre()).c_str()));
	newPlayerElement->InsertEndChild(oreElement);
	
	XMLElement* wheatElement = xmldoc.NewElement("wheat");
	wheatElement->InsertEndChild(xmldoc.NewText(toString(player.getWheat()).c_str()));
	newPlayerElement->InsertEndChild(wheatElement);
	
	XMLElement* woolElement = xmldoc.NewElement("wool");
	woolElement->InsertEndChild(xmldoc.NewText(toString(player.getWool()).c_str()));
	newPlayerElement->InsertEndChild(woolElement);
	
	playersElement->InsertEndChild(newPlayerElement);
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
	
	XMLElement* tileValueElement = xmldoc.NewElement("value");
	tileValueElement->InsertEndChild(xmldoc.NewText(toString(tile.getDiceValue()).c_str()));
	newTileElement->InsertEndChild(tileValueElement);
	
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

Coordinate xmlElementToCoord(const XMLElement& elem) {
	return Coordinate(elem.IntAttribute("u"), elem.IntAttribute("v"));
}

