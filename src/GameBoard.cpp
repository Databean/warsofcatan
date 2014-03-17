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
#include "CornerPiece.h"
#include "City.h"

using std::shared_ptr;
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
			unique_ptr<Player> player(new Player(playerElement));
			players.emplace_back(std::move(player));
		}
	}
	
	auto roadElements = doc.RootElement()->FirstChildElement("roads");
	if(roadElements) {
		for(auto roadElement = roadElements->FirstChildElement(); roadElement; roadElement = roadElement->NextSiblingElement()) {
			Coordinate start = xmlElementToCoord(*(roadElement->FirstChildElement("start")->FirstChildElement("coordinate")));
			Coordinate end = xmlElementToCoord(*(roadElement->FirstChildElement("end")->FirstChildElement("coordinate")));
			std::string ownerName = roadElement->FirstChildElement("owner")->FirstChild()->Value();
			Player* owner = nullptr;
			for(auto& playerUnique : players) {
				if(playerUnique->getName() == ownerName) {
					owner = playerUnique.get();
				}
			}
			if(owner == nullptr) {
				throw std::runtime_error("Road is owned by a nonexistant player.");
			}
			std::shared_ptr<Road> newRoad(new Road(start, end, *owner));
			roads[start].push_back(newRoad);
			roads[end].push_back(newRoad);
		}
	}
}

GameBoard::~GameBoard() {
	
}

/**
 * Find and remove the road that matches startRoad
 */
void GameBoard::removeRoadEnd(std::shared_ptr<Road> startRoad){
	std::vector<shared_ptr<Road>> endRoadVector = roads[startRoad->getEnd()];
	for(std::vector<shared_ptr<Road>>::iterator endRoad = endRoadVector.begin(); endRoad != endRoadVector.end(); endRoad++){
		if((*endRoad) == startRoad){
			(*endRoad) = nullptr;
		}
	}
}

void GameBoard::save(ostream& out) {
	XMLVisitor saver;
	accept(saver);
	tinyxml2::XMLPrinter printer;
	saver.getXMLDoc().Print(&printer);
	out << printer.CStr();
}

const map<Coordinate, unique_ptr<ResourceTile>>& GameBoard::getResources() const {
	return resources;
}

std::vector<Settlement*> GameBoard::GetNeighboringSettlements(
		Coordinate location) const {
	static Coordinate adjacentCoordDiffs[] = { Coordinate(0, 1), Coordinate(1,
			0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0),
			Coordinate(-1, 1) };
	std::vector<Settlement*> v;
	for (unsigned int i = 0; i < 6; i++) {
		const Coordinate& diff = adjacentCoordDiffs[i];
		Coordinate adjacentPoint(location.first + diff.first,
				location.second + diff.second);
		auto it = corners.find(adjacentPoint);
		if (it != corners.end()) {
			GamePiece* piece = it->second.get();
			if (dynamic_cast<Settlement*>(piece)) {
				v.push_back(static_cast<Settlement*>(piece));
			}
		}
	}
	return v;
}

std::vector<CornerPiece*> GameBoard::GetNeighboringCorners(
		Coordinate location) const{
	static Coordinate adjacentCoordDiffs[] = { Coordinate(0, 1), Coordinate(1,
			0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0),
			Coordinate(-1, 1) };
	std::vector<CornerPiece*> v;
	for (unsigned int i = 0; i < 6; i++) {
		const Coordinate& diff = adjacentCoordDiffs[i];
		Coordinate adjacentPoint(location.first + diff.first,
				location.second + diff.second);
		auto it = resources.find(adjacentPoint);
		if (it != resources.end()) {
			GamePiece* piece = it->second.get();
			if (dynamic_cast<CornerPiece*>(piece)) {
				v.push_back(static_cast<CornerPiece*>(piece));
			}
		}
	}
	return v;
}


/**
 * Checks to make sure the coordinate is within bounds of the board
 */
bool GameBoard::outOfBounds(const Coordinate& coord) {
	/**
	 * This code is embarrassing, but I couldn't really figure out how to easily check for out of bounds
	 * I'm sure there is a simple algebraic function that does it, but I went for the hacky way.
	 *
	 * Discussed that we can just do a find in the map, and if it's not found then it's out of bounds
	 */

	switch (coord.second) {
	case 0:
		return !(coord.first >= 0 && coord.first <= 4);
		break;
	case 1:
		return !(coord.first >= -2 && coord.first <= 5);
		break;
	case 2:
		return !(coord.first >= -3 && coord.first <= 5);
		break;
	case 3:
		return !(coord.first >= -3 && coord.first <= 4);
		break;
	case 4:
		return !(coord.first >= -4 && coord.first <= 4);
		break;
	case 5:
		return !(coord.first >= -4 && coord.first <= 3);
		break;
	case 6:
		return !(coord.first >= -5 && coord.first <= 3);
		break;
	case 7:
		return !(coord.first >= -5 && coord.first <= 2);
		break;
	case 8:
		return !(coord.first >= -4 && coord.first <= 0);
		break;
	default:
		break;
	}
	return true;
}

/**
 * Checks to make sure the road doesn't already exist. If it does, then we don't want to add it again
 */
bool GameBoard::roadExists(Coordinate start, Coordinate end) {
	std::shared_ptr<Road> isRoad = getRoad(start, end);
	if (isRoad == NULL)
		return false;
	return true;
}

/**
 * Checks to make sure the road being placed at a valid point according to the rules
 */
bool GameBoard::isRoadConnectionPoint(Coordinate start, Coordinate end, Player& Owner){
	
	//std::unique_ptr<GamePiece> corner = corners[start];
	if(corners[start] != NULL){
		if (corners[start]->getOwner() == Owner)
			return true;

	}
	

	return true;
	
	
}

/**
 * Runs a series of checks to make sure the road can be placed
 */
bool GameBoard::verifyRoadPlacement(Coordinate start, Coordinate end, Player& Owner) {
	if (outOfBounds(start) || outOfBounds(end))
		return false;

	if (roadExists(start, end))
		return false;

	if (!isRoadConnectionPoint(start, end, Owner))
		return false;

	return true;
}

void GameBoard::moveRobber(Coordinate newRobber) {

	robber = newRobber;

	//force trade	
}

Coordinate GameBoard::getRobber() const {
	return robber;

}

/**
 * Places a road at the specified coordinates that will be owned by the given player
 */
void GameBoard::PlaceRoad(Coordinate start, Coordinate end, Player& Owner) {
	if (!verifyRoadPlacement(start, end, Owner))
		return;

	std::shared_ptr<Road> newRoad;
	try {
		newRoad = std::shared_ptr<Road>(new Road(start, end, Owner));
	} catch (int n) {
		//Coordinates did not meet the criteria for a valid road
		return;
	}
	std::vector<shared_ptr<Road>> roadVector = roads[start];
	roadVector.push_back(newRoad);
	roads[start] = roadVector;

	roadVector = roads[end];
	roadVector.push_back(newRoad);
	roads[end] = roadVector;
}

/**
 * returns a pointer to the road located at the specified coordinates. Will return NULL if the road is not found
 */
std::shared_ptr<Road> GameBoard::getRoad(Coordinate start, Coordinate end){
	std::vector<shared_ptr<Road>> roadVector = roads[start];
	for (std::vector<shared_ptr<Road>>::iterator road = roadVector.begin(); road != roadVector.end(); ++road) {
		if ((*road)->equals(start, end))
			return *road;
	}
	return NULL;
}

/**
 * Parent function for the find longest road traversal. Note that longest path is NP-Hard, so there is no simple algorithm for this.
 */
int GameBoard::FindLongestRoad(Player & owner){
	int longest_path = 0;
	//for each road vertex v on the board
	for (auto roadVector = roads.begin(); roadVector != roads.end(); ++roadVector){
		//find the longest path from v
		std::map<Coordinate, bool> marked;
		Coordinate start = roadVector->first;
		int temp_longest_path = FindLongestRoad_FromPoint(start, owner, marked, 0);

		//if that path is longer than the current longest, set to the longest
		if (temp_longest_path > longest_path)
			longest_path = temp_longest_path;
	}

	return longest_path;
}


int GameBoard::FindLongestRoad_FromPoint(Coordinate curr, Player & owner, std::map<Coordinate, bool>& marked, int length){
	marked[curr] = true;
	int longest_path = length;
	//traverse all the surrounding edges and vertices
	std::vector<shared_ptr<Road>> roadVector = roads[curr];
	for (std::vector<shared_ptr<Road>>::iterator road = roadVector.begin(); road != roadVector.end(); ++road) {
		int temp_longest_path = length;

		//if the owner is correct and the road is unmarked
		if ( !(*road)->isMarked() && (*road)->owner->getName().compare(owner.getName()) == 0){

			temp_longest_path++;
			(*road)->mark();
			//Check if you can traverse to the next vertex and make that step if you can
			if(curr != (*road)->getStart() && !marked[(*road)->getStart()]){
				temp_longest_path = FindLongestRoad_FromPoint((*road)->getStart(), owner, marked, temp_longest_path);
			}else if (curr != (*road)->getEnd() && !marked[(*road)->getEnd()]){
				temp_longest_path = FindLongestRoad_FromPoint((*road)->getEnd(), owner, marked, temp_longest_path);
			}
			(*road)->unmark();
		}

		if(temp_longest_path > longest_path)
			longest_path = temp_longest_path;
	}
	marked[curr] = false;
	return longest_path;
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
	corners[location] = std::unique_ptr<CornerPiece>(new Settlement(*this, location, Owner));
}

void GameBoard::UpgradeSettlement(Coordinate location){
	corners[location] = std::unique_ptr<CornerPiece>(new City(*corners[location])); //TODO test for memory leak
}



void GameBoard::accept(GameVisitor& visitor) {
	visitor.visit(*this);
	for(auto& it : corners) {
		if(it.second.get()) {
			it.second->accept(visitor);
		}
	}
	for(auto& it : resources) {
		if(it.second.get()) {
			it.second->accept(visitor);
		}
	}
	for(auto& roadCoordVec : roads) {
		for(auto& road : roadCoordVec.second) {
			if(road.get()) {
				road->accept(visitor);
			}
		}
	}
	for(auto& it : players) {
		if(it.get()) {
			it->accept(visitor);
		}
	}
}

bool GameBoard::operator==(const GameBoard& other) const {
	for(auto& it : corners) {
		auto otherIt = other.corners.find(it.first);
		if(otherIt == other.corners.end()) {
			if(it.second.get()) {
				return false; // This location isn't in the other array
			}
		} else if(!(*(it.second) == *(otherIt->second))) {
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
	for(auto& roadCoordVec : roads) {
		const auto& otherVecIt = other.roads.find(roadCoordVec.first);
		if(otherVecIt == other.roads.end()) {
			return false;
		}
		auto& otherCoordVec = *otherVecIt;
		if(roadCoordVec.second.size() != otherCoordVec.second.size()) {
			return false;
		}
		for(size_t i = 0; i < roadCoordVec.second.size(); i++) {
			const Road& myRoad = *(roadCoordVec.second[i]);
			const Road& otherRoad = *(otherCoordVec.second[i]);
			if(myRoad == otherRoad) {}
			else {
				return false;
			}
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
    this->resources[Coordinate(x,y)] = std::unique_ptr<ResourceTile>(new ResourceTile(*this, Coordinate(x,y), res, val));
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
