#include "GameBoard.h"

#include <map>
#include <memory>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "GameVisitor.h"
#include "Serialization.h"
#include "tinyxml2.h"

using std::random_shuffle;
using std::time;
using std::string;
using std::map;
using std::unique_ptr;
using std::istream;
using std::ostream;
using std::vector;

GameBoard::GameBoard(vector<unique_ptr<Player>>&& players) : players(std::move(players)) {
	init_resources();
}

GameBoard::GameBoard(istream& in) {
	std::string gameXML;
	std::getline(in, gameXML, '\0'); //Read until the null character (end of file) and put in the string
	
	tinyxml2::XMLDocument doc;
	doc.Parse(gameXML.c_str());
	
	auto hexTiles = doc.RootElement()->FirstChildElement("tiles");
	
	for(auto tileElement = hexTiles->FirstChildElement(); tileElement; tileElement = tileElement->NextSiblingElement()) {
		static const map<std::string, resourceType> textToType = {
			std::make_pair("wheat", WHEAT),
			std::make_pair("sheep", SHEEP),
			std::make_pair("stone", STONE),
			std::make_pair("brick", BRICK),
			std::make_pair("wood", WOOD),
			std::make_pair("desert", DESERT),
		};
		std::string typeString = tileElement->FirstChildElement("type")->FirstChild()->Value();
		auto it = textToType.find(typeString);
		if(it == textToType.end()) {
			throw std::runtime_error("Invalid type string");
		}
		resourceType type = it->second;
		
		int diceValue = fromString<int>(tileElement->FirstChildElement("value")->FirstChild()->Value());
		
		Coordinate coord = xmlElementToCoord(*(tileElement->FirstChildElement("coordinate")));
		
		resources[coord] = unique_ptr<ResourceTile>(new ResourceTile(*this, coord, type, diceValue));
	}
	
	auto playerElements = doc.RootElement()->FirstChildElement("players");
	if(playerElements) {
		for(auto playerElement = playerElements->FirstChildElement(); playerElement; playerElement = playerElement->NextSiblingElement()) {
			unique_ptr<Player> player(new Player(playerElement->FirstChildElement("name")->FirstChild()->Value()));
			player->setWood(fromString<int>(playerElement->FirstChildElement("wood")->FirstChild()->Value()));
			player->setBrick(fromString<int>(playerElement->FirstChildElement("brick")->FirstChild()->Value()));
			player->setOre(fromString<int>(playerElement->FirstChildElement("ore")->FirstChild()->Value()));
			player->setWheat(fromString<int>(playerElement->FirstChildElement("wheat")->FirstChild()->Value()));
			player->setWool(fromString<int>(playerElement->FirstChildElement("wool")->FirstChild()->Value()));
			players.emplace_back(std::move(player));
		}
	}
}

GameBoard::~GameBoard() {
	
}

void GameBoard::save(ostream& out) {
	XMLVisitor saver;
	accept(saver);
	tinyxml2::XMLPrinter printer;
	saver.getXMLDoc().Print(&printer);
	out << printer.CStr();
}

const map<Coordinate, unique_ptr<GamePiece>>& GameBoard::getResources() const {
	return resources;
}

std::vector<Settlement*> GameBoard::GetNeighboringSettlements(Coordinate location) {
	static Coordinate adjacentCoordDiffs[] = {Coordinate(0, 1), Coordinate(1, 0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0), Coordinate(-1, 1)};
	std::vector<Settlement*> v;
	for(unsigned int i = 0; i < 6; i++) {
		const Coordinate& diff = adjacentCoordDiffs[i];
		Coordinate adjacentPoint(location.first + diff.first, location.second + diff.second);
		auto it = resources.find(adjacentPoint);
		if(it != resources.end()) {
			GamePiece* piece = it->second.get();
			if(dynamic_cast<Settlement*>(piece)) {
				v.push_back(static_cast<Settlement*>(piece));
			}
		}
	}
	return v;
}

/* 
 *   Initialize board with a set of resources. 
 *   Currently only the standard configuration (no custom shapes or expansion packs) is implemented.
 *   Board tiles and roll numbers are randomized.
 */

void GameBoard::init_resources()
{
    std::srand(std::time(0));
    
    resourceType resources[] = {BRICK, BRICK, BRICK, STONE, STONE, STONE, WHEAT, WHEAT, WHEAT, WHEAT, WOOD, WOOD, WOOD, WOOD, SHEEP, SHEEP, SHEEP, SHEEP, DESERT};
    random_shuffle(&resources[0], &resources[19]);
    
    int rolls[] = {0, 2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    while (!checkRolls(rolls))
    {
        random_shuffle(&rolls[0], &rolls[18]);
    }
    
    int xcoords[] = {-2, 0, 2, -3, -1, 1, 3, -4, -2, 0, 2, 4, -3, -1, 1, 3, -2, 0, 2};
    int ycoords[] = { 2, 1, 0,  4,  3, 2, 1,  6, 5,  4, 3, 2,  7,  6, 5, 4,  8, 7, 6};
	
    
    
    int resourceCount = 0;
    for (int i = 0; i<19; i++)
    {
        if (rolls[i] == 0)
        {
            addResource(xcoords[i], ycoords[i], resources[18], 0);
        }
        else
        {
            addResource(xcoords[i], ycoords[i], resources[resourceCount], rolls[i]);
            resourceCount++;
        }
    }
    
}

void GameBoard::PlaceSettlement(Coordinate location, Player& Owner){
	corners[location] = std::unique_ptr<GamePiece>(new Settlement(*this, location, Owner));
}

void GameBoard::accept(GameVisitor& visitor) {
	for(auto& it : corners) {
		it.second->accept(visitor);
	}
	for(auto& it : resources) {
		it.second->accept(visitor);
	}
	for(auto& it : roads) {
		it->accept(visitor);
	}
	for(auto& it : players) {
		it->accept(visitor);
	}
	visitor.visit(*this);
}

bool GameBoard::operator==(const GameBoard& other) const {
	if(corners.size() != other.corners.size()) {
		return false;
	}
	for(auto& it : corners) {
		auto otherIt = other.corners.find(it.first);
		if(otherIt == other.corners.end()) {
			return false; // This location isn't in the other array
		}
		if(!(*(it.second) == *(otherIt->second))) {
			return false;
		}
	}
	for(auto& it : resources) {
		auto otherIt = other.resources.find(it.first);
		if(otherIt == other.resources.end()) {
			return false; // This location isn't in the other array
		}
		if(!(*(it.second) == *(otherIt->second))) {
			return false;
		}
	}
	for(auto& it : roads) {
		bool hasIt = false;
		for(auto& otherIt : other.roads) {
			if(*it == *otherIt) {
				hasIt = true;
			}
		}
		if(hasIt == false) {
			return false;
		}
	}
	if(players.size() != other.players.size()) {
		return false;
	}
	for(unsigned int i = 0; i < players.size(); i++) {
		if(!(*(players[i]) == *(other.players[i]))) {
			return false;
		}
	}
	return true;
}
/*
 *  Adds a resource and roll tile combo to the board
 *
 *  @param x The first coordinate
 *  @param y The second coordinate
 *  @param res The resource type to be added
 *  @param val The roll tile to be added
 */
void GameBoard::addResource(int x, int y, resourceType res, int val)
{
    this->resources[Coordinate(x,y)] = std::unique_ptr<GamePiece>(new ResourceTile(*this, Coordinate(x,y), res, val));
}

/*
 *  This method checks the array of roll tiles
 *  and makes sure that there are no 6s or 8s next to eachother
 *  for the placement scheme defined in init_resources.
 *
 *  P.S. It's a giant clusterf...well, if you have any ideas on refactoring, please do
 *
 *  @param rolls The array to check
 */
bool GameBoard::checkRolls(int* rolls)
{
    if (rolls[0] == 6 || rolls[0] == 8)
    {
        if (rolls[1] == 6 || rolls[1] == 8)
            return false;
        if (rolls[3] == 6 || rolls[3] == 8)
            return false;
        if (rolls[4] == 6 || rolls[4] == 8)
            return false;
    }
    
    if (rolls[1] == 6 || rolls[1] == 8)
    {
        if (rolls[2] == 6 || rolls[2] == 8)
            return false;
        if (rolls[4] == 6 || rolls[4] == 8)
            return false;
        if (rolls[5] == 6 || rolls[5] == 8)
            return false;
    }
    
    if (rolls[2] == 6 || rolls[2] == 8)
    {
        if (rolls[5] == 6 || rolls[5] == 8)
            return false;
        if (rolls[6] == 6 || rolls[6] == 8)
            return false;
    }
    
    if (rolls[3] == 6 || rolls[3] == 8)
    {
        if (rolls[7] == 6 || rolls[7] == 8)
            return false;
        if (rolls[8] == 6 || rolls[8] == 8)
            return false;
        if (rolls[4] == 6 || rolls[4] == 8)
            return false;
    }
    
    if (rolls[4] == 6 || rolls[4] == 8)
    {
        if (rolls[8] == 6 || rolls[8] == 8)
            return false;
        if (rolls[9] == 6 || rolls[9] == 8)
            return false;
        if (rolls[5] == 6 || rolls[5] == 8)
            return false;
    }
    
    if (rolls[5] == 6 || rolls[5] == 8)
    {
        if (rolls[9] == 6 || rolls[9] == 8)
            return false;
        if (rolls[10] == 6 || rolls[10] == 8)
            return false;
        if (rolls[6] == 6 || rolls[6] == 8)
            return false;
    }
    
    if (rolls[6] == 6 || rolls[6] == 8)
    {
        if (rolls[10] == 6 || rolls[10] == 8)
            return false;
        if (rolls[11] == 6 || rolls[11] == 8)
            return false;
    }
    
    if (rolls[7] == 6 || rolls[7] == 8)
    {
        if (rolls[12] == 6 || rolls[12] == 8)
            return false;
        if (rolls[8] == 6 || rolls[8] == 8)
            return false;
    }
    
    if (rolls[8] == 6 || rolls[8] == 8)
    {
        if (rolls[12] == 6 || rolls[12] == 8)
            return false;
        if (rolls[13] == 6 || rolls[13] == 8)
            return false;
        if (rolls[9] == 6 || rolls[9] == 8)
            return false;
    }
    
    if (rolls[9] == 6 || rolls[9] == 8)
    {
        if (rolls[13] == 6 || rolls[13] == 8)
            return false;
        if (rolls[14] == 6 || rolls[14] == 8)
            return false;
        if (rolls[10] == 6 || rolls[10] == 8)
            return false;
    }
    
    if (rolls[10] == 6 || rolls[10] == 8)
    {
        if (rolls[14] == 6 || rolls[14] == 8)
            return false;
        if (rolls[15] == 6 || rolls[15] == 8)
            return false;
        if (rolls[11] == 6 || rolls[11] == 8)
            return false;
    }
    
    if (rolls[11] == 6 || rolls[11] == 8)
    {
        if (rolls[15] == 6 || rolls[15] == 8)
            return false;
    }
    
    if (rolls[12] == 6 || rolls[12] == 8)
    {
        if (rolls[16] == 6 || rolls[16] == 8)
            return false;
        if (rolls[13] == 6 || rolls[13] == 8)
            return false;
    }
    
    if (rolls[13] == 6 || rolls[13] == 8)
    {
        if (rolls[16] == 6 || rolls[16] == 8)
            return false;
        if (rolls[17] == 6 || rolls[17] == 8)
            return false;
        if (rolls[14] == 6 || rolls[14] == 8)
            return false;
    }
    
    if (rolls[14] == 6 || rolls[14] == 8)
    {
        if (rolls[17] == 6 || rolls[17] == 8)
            return false;
        if (rolls[18] == 6 || rolls[18] == 8)
            return false;
        if (rolls[15] == 6 || rolls[15] == 8)
            return false;
    }
    
    if (rolls[15] == 6 || rolls[15] == 8)
    {
        if (rolls[18] == 6 || rolls[18] == 8)
            return false;
    }
    
    if (rolls[16] == 6 || rolls[16] == 8)
    {
        if (rolls[17] == 6 || rolls[17] == 8)
            return false;
    }
    
    if (rolls[17] == 6 || rolls[17] == 8)
    {
        if (rolls[18] == 6 || rolls[18] == 8)
            return false;
    }
    
    return true;
}

bool GameBoard::testRollChecking(int* rolls)
{
    return checkRolls(rolls);
}
