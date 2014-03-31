#include "GameBoard.h"

#include <map>
#include <memory>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <set>

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

/* 
 *   Initialize board with a set of resources. 
 *   Currently only the standard configuration (no custom shapes or expansion packs) is implemented.
 *   Board tiles and roll numbers are randomized.
 */
GameBoard::GameBoard(vector<unique_ptr<Player>>&& players) : players(std::move(players)) {
	std::srand(std::time(0));
	
	const static vector<resourceType> boardResources {BRICK, BRICK, BRICK, STONE, STONE, STONE, WHEAT, WHEAT, WHEAT, WHEAT, WOOD, WOOD, WOOD, WOOD, SHEEP, SHEEP, SHEEP, SHEEP};
	const static vector<int> boardRolls = {0, 2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
	
	bool valid = false;
	
	const static Coordinate center {0, 4};
	
	while(!valid) {
		this->resources.clear();
		
		vector<resourceType> resources = boardResources;
		random_shuffle(resources.begin(), resources.end());
		
		vector<int> rolls = boardRolls;
		random_shuffle(rolls.begin(), rolls.end());
		
		insertTile(center, resources, rolls);
		for(int i = 1; i < 3; i++) {
			createRing({center.first + i, center.second + i}, i, resources, rolls);
		}
		valid = isValidBoard();
	}
}

void GameBoard::createRing(Coordinate topRight, int sideLength, vector<resourceType>& resources, vector<int>& rolls) {
	//static const Coordinate adjacentTileOffsets[] = {Coordinate(1, -2), Coordinate(2, -1), Coordinate(-1, -1), Coordinate(-2, 1), Coordinate(2, -1), Coordinate(1, 1)};
	static const Coordinate adjacentTileOffsets[] = {Coordinate{1, -2}, Coordinate{-1, -1}, Coordinate{-2, 1}, Coordinate{-1, 2}, Coordinate{1, 1}, Coordinate{2, -1}};
	
	Coordinate coord{topRight};
	for(const Coordinate& offset : adjacentTileOffsets) {
		for(int i = 0; i < sideLength; i++) {
			insertTile(coord, resources, rolls);
			coord.first += offset.first;
			coord.second += offset.second;
		}
	}
}

void GameBoard::insertTile(Coordinate location, vector<resourceType>& resources, vector<int>& rolls) {
	if(rolls.back() == 0) {
		addResource(location.first, location.second, DESERT, rolls.back());
		rolls.pop_back();
	} else {
		addResource(location.first, location.second, resources.back(), rolls.back());
		resources.pop_back();
		rolls.pop_back();
	}
}

GameBoard::GameBoard(std::vector<std::unique_ptr<Player>>&& players, const std::map<Coordinate, std::pair<resourceType, int>>& resourceLocations) : players(std::move(players)) {
	for(auto& resource : resourceLocations) {
		resources[resource.first] = std::unique_ptr<ResourceTile>(new ResourceTile(*this, resource.first, resource.second.first, resource.second.second));
	}
	if(!isValidBoard()) {
		throw std::runtime_error("Board is invalid.");
	}
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
	
	auto settlementElements = doc.RootElement()->FirstChildElement("settlements");
	if(settlementElements) {
		for(auto settlementElement = settlementElements->FirstChildElement(); settlementElement; settlementElement = settlementElement->NextSiblingElement()) {
			Coordinate location = xmlElementToCoord(*(settlementElement->FirstChildElement("coordinate")));
			
			std::string ownerName = settlementElement->FirstChildElement("owner")->FirstChild()->Value();
			Player* owner = nullptr;
			for(auto& playerUnique : players) {
				if(playerUnique->getName() == ownerName) {
					owner = playerUnique.get();
				}
			}
			if(owner == nullptr) {
				throw std::runtime_error("Road is owned by a nonexistant player.");
			}
			PlaceSettlement(location, *owner);
		}
	}
	
	auto cityElements = doc.RootElement()->FirstChildElement("cities");
	if(cityElements) {
		for(auto cityElement = cityElements->FirstChildElement(); cityElement; cityElement = cityElement->NextSiblingElement()) {
			Coordinate location = xmlElementToCoord(*(cityElement->FirstChildElement("coordinate")));
			
			std::string ownerName = cityElement->FirstChildElement("owner")->FirstChild()->Value();
			Player* owner = nullptr;
			for(auto& playerUnique : players) {
				if(playerUnique->getName() == ownerName) {
					owner = playerUnique.get();
				}
			}
			if(owner == nullptr) {
				throw std::runtime_error("Road is owned by a nonexistant player.");
			}
			PlaceCity(location, *owner);
		}
	}
	
	if(!isValidBoard()) {
		throw std::runtime_error("Board is invalid.");
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
 * Checks to make sure the coordinate is within bounds of the board and not a resource tile.
 */
bool GameBoard::outOfBounds(const Coordinate& coord) const {
	//All valid coordinates are adjacent to resource tiles.
	static Coordinate adjacentCoordDiffs[] = {Coordinate(0, 1), Coordinate(1, 0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0), Coordinate(-1, 1)};
	for(auto& diff : adjacentCoordDiffs) {
		if(resources.find(Coordinate{coord.first + diff.first, coord.second + diff.second}) != resources.end()) {
			return false;
		}
	}
	return true;
}

/**
 * Checks to make sure the road doesn't already exist. If it does, then we don't want to add it again
 */
bool GameBoard::roadExists(Coordinate start, Coordinate end) const {
	return bool(getRoad(start, end)); // shared_ptr can convert to bool
}

/**
 * Checks to make sure the road being placed at a valid point according to the rules
 */

bool GameBoard::isRoadConnectionPoint(Coordinate point, Player& Owner) const {
	//is there a settlement we can build off of
	auto cornerIt = corners.find(point);
	if(cornerIt != corners.end()){
		const CornerPiece * corner = cornerIt->second.get();
		if(corner != NULL){
			if (corner->getOwner() == Owner)
				return true;
		}
	}
	
	//is there a road we can build off of
	auto roadIt = roads.find(point);
	if(roadIt != roads.end()) {
		const std::vector<shared_ptr<Road>>& roadVector = roadIt->second;
		for (auto road = roadVector.begin(); road != roadVector.end(); ++road) {
			if ((*road)->getOwner() == Owner)
				return true;
		}
	}

	return false;


}

/**
 * Runs a series of checks to make sure the road can be placed
 * new Roads must be in bounds, unique, and connected to an existing road or settlement
 */
bool GameBoard::verifyRoadPlacement(Coordinate start, Coordinate end, Player& Owner) const {
	if (outOfBounds(start) || outOfBounds(end))
		return false;

	if (roadExists(start, end))
		return false;

	if (!isRoadConnectionPoint(start, Owner) && !isRoadConnectionPoint(end, Owner)) //need to XOR
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
 * returns true if the road was placed, false otherwise
 */
bool GameBoard::PlaceRoad(Coordinate start, Coordinate end, Player& Owner) {
	if (!verifyRoadPlacement(start, end, Owner))
		return false;

	std::shared_ptr<Road> newRoad;
	try {
		newRoad = std::shared_ptr<Road>(new Road(start, end, Owner));
	} catch (int n) {
		//Coordinates did not meet the criteria for a valid road
		return false;
	}
	
	roads[start].push_back(newRoad);
	roads[end].push_back(newRoad);
	
    startTurn();
    
	return true;
}

/**
 * Will purchase a road for the given Player if it is possible.
 * returns true if the road was purchased and placed, false otherwise
 */
bool GameBoard::buyRoad(Coordinate start, Coordinate end, Player& Owner){
	if(Owner.canBuyRoad() && PlaceRoad(start, end, Owner)){
		Owner.buyRoad();
		return true;
	}
	return false;
}


/**
 * returns a pointer to the road located at the specified coordinates. Will return NULL if the road is not found
 */
const std::shared_ptr<Road> GameBoard::getRoad(Coordinate start, Coordinate end) const {
	auto roadVecIt = roads.find(start);
	if(roadVecIt != roads.end()) {
		for (auto road = roadVecIt->second.begin(); road != roadVecIt->second.end(); road++) {
			if ((*road)->equals(start, end))
				return *road;
		}
	}
	return NULL;
}

const std::vector<std::shared_ptr<Road>>& GameBoard::getRoads(Coordinate coord) const {
	static const std::vector<std::shared_ptr<Road>> empty;
	if(roads.find(coord) != roads.end()) {
		return roads.find(coord)->second;
	}
	return empty;
}

/**
 * Parent function for the find longest road traversal. Note that longest path is NP-Hard, so there is no simple algorithm for this.
 */
int GameBoard::FindLongestRoad(const Player & owner) const {
	int longest_path = 0;
	//for each road vertex v on the board
	for (auto roadVector = roads.begin(); roadVector != roads.end(); ++roadVector){
		//find the longest path from v
		std::map<Coordinate, bool> marked;
		std::map<Road*, bool> markedRoads;
		Coordinate start = roadVector->first;
		int temp_longest_path = FindLongestRoad_FromPoint(start, owner, marked, markedRoads, 0);

		//if that path is longer than the current longest, set to the longest
		if (temp_longest_path > longest_path)
			longest_path = temp_longest_path;
	}

	return longest_path;
}


int GameBoard::FindLongestRoad_FromPoint(Coordinate curr, const Player & owner, std::map<Coordinate, bool>& marked, std::map<Road*, bool>& markedRoads, int length) const {
	marked[curr] = true;
	int longest_path = length;
	//traverse all the surrounding edges and vertices
	auto roadVectorIt = roads.find(curr);
	if(roadVectorIt != roads.end()) {
	auto& roadVector = roadVectorIt->second;
	for (auto road = roadVector.begin(); road != roadVector.end(); ++road) {
		int temp_longest_path = length;

		//if the owner is correct and the road is unmarked
		if ( !markedRoads[road->get()] && (*road)->getOwner().getName() == owner.getName()){

			temp_longest_path++;
			markedRoads[road->get()] = true;
			//Check if you can traverse to the next vertex and make that step if you can
			if(curr != (*road)->getStart() && !marked[(*road)->getStart()]){
				temp_longest_path = FindLongestRoad_FromPoint((*road)->getStart(), owner, marked, markedRoads, temp_longest_path);
			}else if (curr != (*road)->getEnd() && !marked[(*road)->getEnd()]){
				temp_longest_path = FindLongestRoad_FromPoint((*road)->getEnd(), owner, marked, markedRoads, temp_longest_path);
			}
			markedRoads[road->get()] = false;
		}

		if(temp_longest_path > longest_path)
			longest_path = temp_longest_path;
		}
	}
	marked[curr] = false;
	return longest_path;
}



void GameBoard::PlaceSettlement(Coordinate location, Player& Owner){
	corners[location] = std::unique_ptr<CornerPiece>(new Settlement(*this, location, Owner));
}

void GameBoard::PlaceCity(Coordinate location, Player& Owner){
	corners[location] = std::unique_ptr<CornerPiece>(new City(*this, location, Owner));

}

void GameBoard::UpgradeSettlement(Coordinate location){
	corners[location] = std::unique_ptr<CornerPiece>(new City(*corners[location])); //TODO test for memory leak
}



void GameBoard::accept(GameVisitor& visitor) {
	visitor.visit(*this);


	// Drawing needs this to happen in this order. Visitors technically should be order-independent, but this was an easy fix at the moment.
	// Keep that in mind when modifying this.
	for(auto& it : resources) {
		it.second->accept(visitor);
	}
	for(auto& it : corners) {
		it.second->accept(visitor);

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
	for(auto& it : corners) {
		auto otherIt = other.corners.find(it.first);
		if(otherIt == other.corners.end()) {
			return false;
		} else if(!(*it.second == *otherIt->second)) {
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

bool GameBoard::isValidBoard() const {
	for(auto& resourcePair : resources) {
		const ResourceTile& tile = dynamic_cast<const ResourceTile&>(*resourcePair.second);
		
		if(tile.getDiceValue() != 6 && tile.getDiceValue() != 8) {
			continue;
		}
		
		static const Coordinate adjacentTileOffsets[] = {Coordinate(1, 1), Coordinate(2, -1), Coordinate(1, -2), Coordinate(-1, -1), Coordinate(-2, 1), Coordinate(2, -1)};
		for(const Coordinate& offset : adjacentTileOffsets) {
			Coordinate adjacentTileCoord(tile.getLocation().first + offset.first, tile.getLocation().second + offset.second);
			auto otherCoordinateIt = resources.find(adjacentTileCoord);
			if(otherCoordinateIt != resources.end() && otherCoordinateIt->second) {
				const ResourceTile& otherTile = dynamic_cast<const ResourceTile&>(*otherCoordinateIt->second);
				if(otherTile.getDiceValue() == 6 || otherTile.getDiceValue() == 8) {
					return false;
				}
			}
		}
	}
	return true;
}

const std::vector<std::unique_ptr<Player>>& GameBoard::getPlayers() const {
	return players;
}

/*
 *  When a player begins their turn, this rolls the dice and takes the required action (paying resources or enabling robber movement)
 *  @return An array of the dice rolls
 */
std::pair<int, int> GameBoard::startTurn()
{
    int die1 = std::rand() % 6 + 1;
    int die2 = std::rand() % 6 + 1;
    int roll = die1+die2;
    std::cout << "\nDie 1: " << die1 << "\nDie 2: " << die2 << "\nRoll: " << roll <<"\n";
    
    if (roll==7)
        enableRobber();
    
    else
        payoutResources(roll);
    
    return std::make_pair(die1, die2);
}

/*
 *  When a 7 is rolled, this enforces resource discarding and allows the current player to move the robber
 */
void GameBoard::enableRobber()
{
    //Do some straight up robber stuff.
}

/*
 *  This pays resources based on the current roll
 */
void GameBoard::payoutResources(int roll)
{
    
}
