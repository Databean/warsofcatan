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
#include "GameDice.h"
#include "Settlement.h"
#include "City.h"
#include "Wonder.h"

using std::shared_ptr;
using std::random_shuffle;
using std::time;
using std::string;
using std::map;
using std::unique_ptr;
using std::istream;
using std::ostream;
using std::vector;

/**
 * Initialize board with a set of resources. 
 * Currently only the standard configuration (no custom shapes or expansion packs) is implemented.
 * Board tiles and roll numbers are randomized.
 * @param players A vector of the players playing the game.
 */
GameBoard::GameBoard(const vector<std::string>& playerNames) {
	for (auto& name : playerNames) {
		players.push_back(std::unique_ptr<Player>(new Player(*this, name)));
	}

	currentTurn = 0;
	maxVictoryPoints = 10;
	winner = -1;

	std::srand(std::time(0));

	const static vector<resourceType> boardResources { BRICK, BRICK, BRICK,
			STONE, STONE, STONE, WHEAT, WHEAT, WHEAT, WHEAT, WOOD, WOOD, WOOD,
			WOOD, SHEEP, SHEEP, SHEEP, SHEEP };
	const static vector<int> boardRolls = { 0, 2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8,
			9, 9, 10, 10, 11, 11, 12 };

	bool valid = false;

	const static Coordinate center { 0, 4 };

	while (!valid) {
		this->resources.clear();

		vector<resourceType> resources = boardResources;
		random_shuffle(resources.begin(), resources.end());

		vector<int> rolls = boardRolls;
		random_shuffle(rolls.begin(), rolls.end());

		insertTile(center, resources, rolls);
		for (int i = 1; i < 3; i++) {
			createRing( { center.first + i, center.second + i }, i, resources,
					rolls);
		}
		valid = isValidBoard();
	}
	//moveRobber(Coordinate(0,4));
	auto it = getResources().begin();
	while (it != getResources().end()) {
		if ((it->second)->getType() == DESERT)
			moveRobber(it->first);
		it++;
	}
	std::cout << getRobber().first << "\n";
	std::cout << getRobber().second << "\n";

	maxVictoryPoints = 10;
}

/**
 * Create a ring of hexagons of a particular radius around a center point. Takes in a vector of resource types and a vector of rolls to pull
 * from.
 * @param topRight The top-right hexagon in the ring.
 * @param sideLength The number of hexagons in a particular face of the outer hexagon.
 * @param[out] resources A vector of the resources still available for placement.
 * @param[out] rolls A vector of the rolls still available for placement.
 */
void GameBoard::createRing(Coordinate topRight, int sideLength,
		vector<resourceType>& resources, vector<int>& rolls) {
	//static const Coordinate adjacentTileOffsets[] = {Coordinate(1, -2), Coordinate(2, -1), Coordinate(-1, -1), Coordinate(-2, 1), Coordinate(2, -1), Coordinate(1, 1)};
	static const Coordinate adjacentTileOffsets[] = { Coordinate { 1, -2 },
			Coordinate { -1, -1 }, Coordinate { -2, 1 }, Coordinate { -1, 2 },
			Coordinate { 1, 1 }, Coordinate { 2, -1 } };

	Coordinate coord { topRight };
	for (const Coordinate& offset : adjacentTileOffsets) {
		for (int i = 0; i < sideLength; i++) {
			insertTile(coord, resources, rolls);
			coord.first += offset.first;
			coord.second += offset.second;
		}
	}
}

/**
 * Create a particular hexagon at a location on the board. Takes in a vector of resource types and a vector of rolls to pull from.
 * @param location The coordinate to insert the hexagon on.
 * @param[out] resources The vector of resources still available for placement.
 * @param[out] rolls The vector of rolls still available for placement.
 */
void GameBoard::insertTile(Coordinate location, vector<resourceType>& resources,
		vector<int>& rolls) {
	if (rolls.back() == 0) {
		addResource(location.first, location.second, DESERT, rolls.back());
		rolls.pop_back();
	} else {
		addResource(location.first, location.second, resources.back(),
				rolls.back());
		resources.pop_back();
		rolls.pop_back();
	}
}

/**
 * Construct a board with a pre-built map of resource tiles. Throws an exception if the resource tiles are invalid.
 * @param players The players playing the game.
 * @param resourceLocations A mapping from coordinates to resource types and dice values, representing the tiles.
 * @throws std::runtime_error When the configuration is invalid.
 */
GameBoard::GameBoard(const std::vector<std::string>& playerNames,
		const std::map<Coordinate, std::pair<resourceType, int>>& resourceLocations) {
	for (auto& name : playerNames) {
		players.push_back(std::unique_ptr<Player>(new Player(*this, name)));
	}

	for (auto& resource : resourceLocations) {
		resources[resource.first] = std::unique_ptr<ResourceTile>(
				new ResourceTile(*this, resource.first, resource.second.first,
						resource.second.second));
	}
	if (!isValidBoard()) {
		throw std::runtime_error("Board is invalid.");
	}
	currentTurn = 0;
	maxVictoryPoints = 10;
	winner = -1;
}

GameDice GameBoard::getDice() {
	return dice;
}
/**
 * Construct a board by reading in an XML representation from a stream.
 * @param in The stream to read from.
 */
GameBoard::GameBoard(istream& in) {
	std::string gameXML;
	std::getline(in, gameXML, '\0'); //Read until the null character (end of file) and put in the string

	tinyxml2::XMLDocument doc;
	doc.Parse(gameXML.c_str());

	auto hexTiles = doc.RootElement()->FirstChildElement("tiles");

	for (auto tileElement = hexTiles->FirstChildElement(); tileElement;
			tileElement = tileElement->NextSiblingElement()) {
		static const map<std::string, resourceType> textToType =
				{ std::make_pair("wheat", WHEAT), std::make_pair("sheep",
						SHEEP), std::make_pair("stone", STONE), std::make_pair(
						"brick", BRICK), std::make_pair("wood", WOOD),
						std::make_pair("desert", DESERT), };
		std::string typeString =
				tileElement->FirstChildElement("type")->FirstChild()->Value();
		auto it = textToType.find(typeString);
		if (it == textToType.end()) {
			throw std::runtime_error("Invalid type string");
		}
		resourceType type = it->second;

		int diceValue = fromString<int>(
				tileElement->FirstChildElement("value")->FirstChild()->Value());

		Coordinate coord = xmlElementToCoord(
				*(tileElement->FirstChildElement("coordinate")));

		resources[coord] = unique_ptr<ResourceTile>(
				new ResourceTile(*this, coord, type, diceValue));
	}

	auto playerElements = doc.RootElement()->FirstChildElement("players");
	if (playerElements) {
		for (auto playerElement = playerElements->FirstChildElement();
				playerElement; playerElement =
						playerElement->NextSiblingElement()) {
			unique_ptr<Player> player(new Player(*this, playerElement));
			players.emplace_back(std::move(player));
		}
	}

	auto roadElements = doc.RootElement()->FirstChildElement("roads");
	if (roadElements) {
		for (auto roadElement = roadElements->FirstChildElement(); roadElement;
				roadElement = roadElement->NextSiblingElement()) {
			Coordinate start =
					xmlElementToCoord(
							*(roadElement->FirstChildElement("start")->FirstChildElement(
									"coordinate")));
			Coordinate end = xmlElementToCoord(
					*(roadElement->FirstChildElement("end")->FirstChildElement(
							"coordinate")));
			std::string ownerName =
					roadElement->FirstChildElement("owner")->FirstChild()->Value();
			Player* owner = nullptr;
			for (auto& playerUnique : players) {
				if (playerUnique->getName() == ownerName) {
					owner = playerUnique.get();
				}
			}
			if (owner == nullptr) {
				throw std::runtime_error(
						"Road is owned by a nonexistant player.");
			}
			std::shared_ptr<Road> newRoad(new Road(start, end, *owner));
			roads[start].push_back(newRoad);
			roads[end].push_back(newRoad);
		}
	}

	auto settlementElements = doc.RootElement()->FirstChildElement(
			"settlements");
	if (settlementElements) {
		for (auto settlementElement = settlementElements->FirstChildElement();
				settlementElement;
				settlementElement = settlementElement->NextSiblingElement()) {
			Coordinate location = xmlElementToCoord(
					*(settlementElement->FirstChildElement("coordinate")));

			std::string ownerName = settlementElement->FirstChildElement(
					"owner")->FirstChild()->Value();
			Player* owner = nullptr;
			for (auto& playerUnique : players) {
				if (playerUnique->getName() == ownerName) {
					owner = playerUnique.get();
				}
			}
			if (owner == nullptr) {
				throw std::runtime_error(
						"Settlement is owned by a nonexistant player.");
			}
			PlaceSettlement(location, *owner);
		}
	}

	auto cityElements = doc.RootElement()->FirstChildElement("cities");
	if (cityElements) {
		for (auto cityElement = cityElements->FirstChildElement(); cityElement;
				cityElement = cityElement->NextSiblingElement()) {
			Coordinate location = xmlElementToCoord(
					*(cityElement->FirstChildElement("coordinate")));

			std::string ownerName =
					cityElement->FirstChildElement("owner")->FirstChild()->Value();
			Player* owner = nullptr;
			for (auto& playerUnique : players) {
				if (playerUnique->getName() == ownerName) {
					owner = playerUnique.get();
				}
			}
			if (owner == nullptr) {
				throw std::runtime_error(
						"City is owned by a nonexistant player.");
			}
			PlaceCity(location, *owner);
		}
	}

	auto wonderElements = doc.RootElement()->FirstChildElement("wonders");
	if (wonderElements) {
		for (auto wonderElement = wonderElements->FirstChildElement();
				wonderElement; wonderElement =
						wonderElement->NextSiblingElement()) {
			Coordinate location = xmlElementToCoord(
					*(wonderElement->FirstChildElement("coordinate")));

			std::string ownerName =
					wonderElement->FirstChildElement("owner")->FirstChild()->Value();
			Player* owner = nullptr;
			for (auto& playerUnique : players) {
				if (playerUnique->getName() == ownerName) {
					owner = playerUnique.get();
				}
			}
			if (owner == nullptr) {
				throw std::runtime_error(
						"Wonder is owned by a nonexistant player.");
			}
			PlaceCity(location, *owner);
		}
	}

	if (!isValidBoard()) {
		throw std::runtime_error("Board is invalid.");
	}

	currentTurn = 0; 		//have to update
	maxVictoryPoints = 0;
	winner = -1;
}

/**
 * Destroy the GameBoard.
 */
GameBoard::~GameBoard() {

}

/**
 * Find and remove the road that matches startRoad
 * @param startRoad The road used for comparison to existing roads.
 */
void GameBoard::removeRoadEnd(std::shared_ptr<Road> startRoad) {
	std::vector<shared_ptr<Road>> endRoadVector = roads[startRoad->getEnd()];
	for (std::vector<shared_ptr<Road>>::iterator endRoad =
			endRoadVector.begin(); endRoad != endRoadVector.end(); endRoad++) {
		if ((*endRoad) == startRoad) {
			(*endRoad) = nullptr;
		}
	}
}

/**
 * Serializes the GameBoard to XML on an output stream.
 * @param out The stream to write the serialized board to.
 */
void GameBoard::save(ostream& out) {
	XMLVisitor saver;
	accept(saver);
	tinyxml2::XMLPrinter printer;
	saver.getXMLDoc().Print(&printer);
	out << printer.CStr();
}

/**
 * Retrieve the map of resource tiles.
 * @return The map of resource tiles used internally.
 */
const map<Coordinate, unique_ptr<ResourceTile>>& GameBoard::getResources() const {
	return resources;
}

/**
 * Retrieve the ResourceTile at a particular location.
 * @param location The coordinate to look for
 * @return The resource tile at the given location.
 */
ResourceTile& GameBoard::getResourceTile(Coordinate location) const {
	//return resources.at(location);

	return *(resources.find(location)->second);
}

/**
 * Ends current players turn and moves the current turn marker
 */
void GameBoard::endTurn() {
	std::cout << currentTurn << std::endl;
	if (getCurrentPlayer().getVictoryPoints() >= getMaxVictoryPoints()) {
		//std::cout<<"GG Bitches";
		winner = currentTurn;
	}

	currentTurn++;
	if (currentTurn >= getNoOfPlayers())
		currentTurn = 0;

	startTurn();
}

void GameBoard::initializeGame() {
	PlaceSettlement(Coordinate(-1, 2), getPlayer(0));
	PlaceSettlement(Coordinate(1, 1), getPlayer(0));
	getPlayer(0).setStartingValues();

	PlaceSettlement(Coordinate(-2, 4), getPlayer(1));
	PlaceSettlement(Coordinate(-2, 6), getPlayer(1));
	getPlayer(1).setStartingValues();

	PlaceSettlement(Coordinate(-1, 7), getPlayer(2));
	PlaceSettlement(Coordinate(1, 6), getPlayer(2));
	getPlayer(2).setStartingValues();

	PlaceSettlement(Coordinate(2, 4), getPlayer(3));
	PlaceSettlement(Coordinate(2, 2), getPlayer(3));
	getPlayer(3).setStartingValues();

	for (int i = 1; i < 13; i++) {
		payoutResources(i);
	}

}

/**
 * @return The no of Victory points needed to win the game
 */
int GameBoard::getMaxVictoryPoints() {
	return maxVictoryPoints;
}

/**
 * Sets the no of victory points needed to win the game
 * @param maxVicPts victory points needed to win the game
 */
void GameBoard::setMaxVictoryPoints(int maxVicPts) {
	maxVictoryPoints = maxVicPts;
}

/**
 * Finds settlements neighboring a particular coordinate.
 * @param location The location to search the neighbors of.
 * @return A vector of the settlements in the vicinity.
 */
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

/**
 * Finds corner pieces neighboring a particular coordinate.
 * @param location The location to search the neighbors of.
 * @return A vector of the corner pieces in the vicinity.
 */
std::vector<CornerPiece*> GameBoard::GetNeighboringCorners(
		Coordinate location) const {
	static Coordinate adjacentCoordDiffs[] = { Coordinate(0, 1), Coordinate(1,
			0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0),
			Coordinate(-1, 1) };

	std::vector<CornerPiece*> v;
	for (unsigned int i = 0; i < 6; i++) {
		const Coordinate& diff = adjacentCoordDiffs[i];
		Coordinate adjacentPoint(location.first + diff.first,
				location.second + diff.second);

		auto it = corners.find(adjacentPoint);
		if (it != corners.end()) {
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
 * @param coord The coordinate to check.
 * @return Whether the coordinate is a valid palcement for an object.
 */
bool GameBoard::outOfBounds(const Coordinate& coord) const {
	//All valid coordinates are adjacent to resource tiles.
	static Coordinate adjacentCoordDiffs[] = { Coordinate(0, 1), Coordinate(1,
			0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0),
			Coordinate(-1, 1) };
	for (auto& diff : adjacentCoordDiffs) {
		if (resources.find(
				Coordinate { coord.first + diff.first, coord.second
						+ diff.second }) != resources.end()) {
			return false;
		}
	}
	return true;
}

/**
 * Checks to make sure the road doesn't already exist. If it does, then we don't want to add it again
 * @param start One of the coordinates of the road.
 * @param end The other coordinate of the road
 * @return Whether a road exists between start and end.
 */
bool GameBoard::roadExists(Coordinate start, Coordinate end) const {
	return bool(getRoad(start, end)); // shared_ptr can convert to bool
}

/**
 * Checks if a road can be built with the given endpoint owned by the given player.
 * @param point The point to attempt building a road from.
 * @param Owner The player attempting to build a road.
 * @return If the player can construct a road there.
 */
bool GameBoard::isRoadConnectionPoint(Coordinate point, Player& Owner) const {
	//is there a settlement we can build off of
	auto cornerIt = corners.find(point);
	if (cornerIt != corners.end()) {
		const CornerPiece * corner = cornerIt->second.get();
		if (corner != NULL) {
			if (corner->getOwner() == Owner)
				return true;
		}
	}

	//is there a road we can build off of
	auto roadIt = roads.find(point);
	if (roadIt != roads.end()) {
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
 * @param start One side of the road.
 * @param end The other side of the road.
 * @param Owner The player attempting to build the road.
 * @return If the road can be placed at the locations by the player.
 */
bool GameBoard::verifyRoadPlacement(Coordinate start, Coordinate end,
		Player& Owner) const {
	if (outOfBounds(start) || outOfBounds(end)) {
		std::cout << "out of bounds" << std::endl;
		return false;
	}

	if (roadExists(start, end)) {
		std::cout << "road exists" << std::endl;
		return false;
	}

	if (!isRoadConnectionPoint(start, Owner)
			&& !isRoadConnectionPoint(end, Owner)) { //need to XOR
		std::cout << "not a road connection point" << std::endl;
		return false;
	}

	if (!Road::isValidRoad(start, end)) {
		std::cout << "not a valid road" << std::endl;
		return false;
	}

	return true;
}

/**
 * Move the robber to a new coordinate on the board.
 * @param newRobber The coordinate to move the robber to.
 */
bool GameBoard::moveRobber(Coordinate newRobber) {

	//Bounds check
	if (resources.find(newRobber) != resources.end()) {
		robber = newRobber;
		return true;
	}
	return false;
}

/**
 * Returns whether the robber can rob the Player opponent at the recourse tile Coordinate location
 * @return true if the robber can rob the opponent, false otherwise
 */
bool GameBoard::canRobberRob(Player& opponent, Coordinate location) {
	for (auto corner : GetNeighboringCorners(location)) {
		if (corner->getOwner() == opponent) {
			return true;
		}
	}
	return false;
}

/**
 * The robber's location on the board.
 * @return The robber's location.
 */
Coordinate GameBoard::getRobber() const {
	return robber;

}

/**
 * Places a road at the specified coordinates that will be owned by the given player.
 * @param start One endpoint of the road.
 * @param end The other endpoint of the road.
 * @param Owner The player placing the road.
 * @returns True if the road was placed, false otherwise
 */
bool GameBoard::PlaceRoad(Coordinate start, Coordinate end, Player& Owner) {
	if (!verifyRoadPlacement(start, end, Owner)) {
		std::cout << "invalid road placement" << std::endl;
		return false;
	}

	std::cout << "passed verify" << std::endl;

	std::shared_ptr<Road> newRoad;
	try {
		newRoad = std::shared_ptr<Road>(new Road(start, end, Owner));
	} catch (std::invalid_argument& e) {
		//Coordinates did not meet the criteria for a valid road
		return false;
	}

	roads[start].push_back(newRoad);
	roads[end].push_back(newRoad);

	return true;
}

/**
 * Checks to see if a road building card can be played on the given points. This is essentially an extension of the VerifyRoadPlacement function.
 * @param start1 one end of the first road
 * @param end1 the other end of the first road
 * @param start2 one end of the second road
 * @param end2 the other end of the second road
 * @param Owner the owner of these roads
 * @return true if the road can be placed, false otherwise
 */
bool GameBoard::canPlayBuildRoadCard(Coordinate start1, Coordinate end1,
		Coordinate start2, Coordinate end2, Player& Owner) {
	//Verify the roads are in bounds of the board
	if (outOfBounds(start1) || outOfBounds(end1) || outOfBounds(start2)
			|| outOfBounds(end2))
		return false;

	//Verify the roads do not already exist
	if (roadExists(start1, end1) || roadExists(start2, end2))
		return false;

	//Verify the roads are extending of an existing road in some fashion
	if (start1 == start2 || start1 == end2 || start2 == end1 || end1 == end2) {
		if (!isRoadConnectionPoint(start1, Owner)
				&& !isRoadConnectionPoint(end1, Owner)
				&& !isRoadConnectionPoint(start2, Owner)
				&& !isRoadConnectionPoint(end2, Owner)) {
			return false;
		}
	} else if ((!isRoadConnectionPoint(start1, Owner)
			&& !isRoadConnectionPoint(end1, Owner))
			|| (!isRoadConnectionPoint(start2, Owner)
					&& !isRoadConnectionPoint(end2, Owner))) {
		return false;
	}

	//Verify the roads are valid roads of length 1
	if (!Road::isValidRoad(start1, end1) || !Road::isValidRoad(start2, end2))
		return false;

	return true;
}

/**
 * Will purchase a road for the given Player if it is possible.
 * @param start One endpoint of the road to be placed.
 * @param end The other endpoint of the road to be placed.
 * @param Owner The player placing the road.
 * @return True if the road was purchased and placed, false otherwise
 */
bool GameBoard::buyRoad(Coordinate start, Coordinate end, Player& Owner) {
	if (Owner.canBuyRoad() && PlaceRoad(start, end, Owner)) {
		Owner.buyRoad();
		return true;
	}
	std::cout << "failed to buy for some reason" << std::endl;
	return false;
}

/**
 * @return A pointer to the road located at the specified coordinates, or NULL if the road is not found
 */
const std::shared_ptr<Road> GameBoard::getRoad(Coordinate start,
		Coordinate end) const {
	auto roadVecIt = roads.find(start);
	if (roadVecIt != roads.end()) {
		for (auto road = roadVecIt->second.begin();
				road != roadVecIt->second.end(); road++) {
			if ((*road)->equals(start, end))
				return *road;
		}
	}
	return NULL;
}

/**
 * Retrieves a vector of roads attached to a given Coordinate
 * @param the Coordinate to the corner you want the roads of
 * @return a vector of shared_ptrs of the roads at that Coordinate
 */
const std::vector<std::shared_ptr<Road>>& GameBoard::getRoads(
		Coordinate coord) const {
	static const std::vector<std::shared_ptr<Road>> empty;
	if (roads.find(coord) != roads.end()) {
		return roads.find(coord)->second;
	}
	return empty;
}

/**
 * Parent function for the find longest road traversal. Note that longest path is NP-Hard, so there is no simple algorithm for this.
 * @param owner The player to find the longest road of.
 * @return The length of the longest road.
 */
int GameBoard::FindLongestRoad(const Player & owner) const {
	int longest_path = 0;
	//for each road vertex v on the board
	for (auto roadVector = roads.begin(); roadVector != roads.end();
			++roadVector) {
		//find the longest path from v
		std::map<Coordinate, bool> marked;
		std::map<Road*, bool> markedRoads;
		Coordinate start = roadVector->first;
		int temp_longest_path = FindLongestRoad_FromPoint(start, owner, marked,
				markedRoads, 0);

		//if that path is longer than the current longest, set to the longest
		if (temp_longest_path > longest_path)
			longest_path = temp_longest_path;
	}

	return longest_path;
}

/**
 * Internal function to find the longest road that starts at a particular point.
 * @param curr The coordinate to search from.
 * @param owner The player to find the longest road of.
 * @param[out] marked A reference to the points already visited in this pass.
 * @param[out] markedRoads A reference to the roads already visited in this pass.
 * @param length The length of the road that leads to the point currently being visited.
 * @return The length of the longest road starting at the current point plus the length of the road up to this point.
 */
int GameBoard::FindLongestRoad_FromPoint(Coordinate curr, const Player & owner,
		std::map<Coordinate, bool>& marked, std::map<Road*, bool>& markedRoads,
		int length) const {
	marked[curr] = true;
	int longest_path = length;
	//traverse all the surrounding edges and vertices
	auto roadVectorIt = roads.find(curr);
	if (roadVectorIt != roads.end()) {
		auto& roadVector = roadVectorIt->second;
		for (auto road = roadVector.begin(); road != roadVector.end(); ++road) {
			int temp_longest_path = length;

			//if the owner is correct and the road is unmarked
			if (!markedRoads[road->get()]
					&& (*road)->getOwner().getName() == owner.getName()) {

				temp_longest_path++;
				markedRoads[road->get()] = true;
				//Check if you can traverse to the next vertex and make that step if you can
				if (curr != (*road)->getStart()
						&& !marked[(*road)->getStart()]) {
					temp_longest_path = FindLongestRoad_FromPoint(
							(*road)->getStart(), owner, marked, markedRoads,
							temp_longest_path);
				} else if (curr != (*road)->getEnd()
						&& !marked[(*road)->getEnd()]) {
					temp_longest_path = FindLongestRoad_FromPoint(
							(*road)->getEnd(), owner, marked, markedRoads,
							temp_longest_path);
				}
				markedRoads[road->get()] = false;
			}

			if (temp_longest_path > longest_path)
				longest_path = temp_longest_path;
		}
	}
	marked[curr] = false;
	return longest_path;
}

int GameBoard::CountCornerPoints(Player& owner) {
	int point_sum = 0;

	for (std::map<Coordinate, std::unique_ptr<CornerPiece>>::iterator cp =
			corners.begin(); cp != corners.end(); ++cp) {
		if (cp->second->getOwner() == owner) {
			point_sum += cp->second->getVictoryPoints();
		}
	}
	return point_sum;
}

/**
 * Cycles through the Players and finds who has the longest road and sets their status to reflect that.
 * The Player with the longest road is the player who has the longest string of connected roads.
 * It must be at least 5 long. In cases of ties, the Player who reached that length first has the longest road.
 */
void GameBoard::updateLongestRoadPlayer() {
	int longestRoad = -1;
	int longestPlayer = 0;

	for (int i = 0; i < getNoOfPlayers(); i++) {
		if (getPlayer(i).hasLongestRoad()) {
			longestPlayer = i;
		}

		getPlayer(i).setLongestRoadSize(FindLongestRoad(getPlayer(i)));
		getPlayer(i).setLongestRoad(false);
		if (getPlayer(i).getLongestRoadSize() > longestRoad) {
			longestRoad = getPlayer(i).getLongestRoadSize();
			longestPlayer = i;
		}
	}

	if (longestRoad >= 5) {
		getPlayer(longestPlayer).setLongestRoad(true);
	}

}

/**
 * Cycles through the Players and finds who has the largest army and sets their status to reflect that.
 * The Player with the largest army is the player who has played the most Knight cards.
 * It must be at least 3 large. In cases of ties, the Player who reached that size first has the largest army.
 */
void GameBoard::updateLargestArmyPlayer() {
	int largestArmy = -1;
	int largestPlayer = 0;

	for (int i = 0; i < getNoOfPlayers(); i++) {
		if (getPlayer(i).hasLargestArmy()) {
			largestPlayer = i;
		}

		getPlayer(i).setLargestArmy(false);
		if (getPlayer(i).getArmySize() > largestArmy) {
			largestArmy = getPlayer(i).getArmySize();
			largestPlayer = i;
		}
	}

	if (largestArmy >= 3) {
		getPlayer(largestPlayer).setLargestArmy(true);
	}

}

/**
 * Whether a player can place a settlement at a location.
 * @param location The place to put the settlement.
 * @param owner The player placing the settlement.
 * @return If the location is a valid place to put a settlement.
 */
bool GameBoard::canPlaceSettlement(const Coordinate& location,
		const Player& owner) {
	//Don't place this on top of a resource
	if (resources.find(location) != resources.end()) {
		std::cout << "can't put settlements on top of resource tiles"
				<< std::endl;
		return false;
	}
	//Don't place this on top of another settlement
	if (corners.find(location) != corners.end()) {
		std::cout << "can't put a settlement on top of another corner piece"
				<< std::endl;
		return false;
	}
	//Don't place this off the map
	if (outOfBounds(location)) {
		std::cout << "this is out of bounds" << std::endl;
		return false;
	}
	//Can't have a settlement next to another settlement.
	if (GetNeighboringCorners(location).size() > 0) {
		std::cout << "there's an adjacent corner piece" << std::endl;
		return false;
	}
	for (auto road : getRoads(location)) {
		if (road->getOwner() == owner) {
			//Player has a connecting road
			return true;
		}
	}
	std::cout << "there are no connecting roads" << std::endl;
	//Player has no connecting roads
	return false;
}

/**
 * Buy a settlement if possible.
 * @param location The location to place the settlement.
 * @param owner The player buying the settlement.
 * @return If placing the settlement was a success.
 */
bool GameBoard::buySettlement(const Coordinate& location, Player& owner) {
	if (canPlaceSettlement(location, owner) && owner.canBuySettlement()) {
		if (!owner.buySettlement()) {
			std::cout << "wat" << std::endl;
			return false;
		}
		PlaceSettlement(location, owner);
		return true;
	}
	return false;
}

/**
 * Place a settlement on the board.
 * @param location Where to place it on the board.
 * @param Owner The player placing the settlement.
 */
void GameBoard::PlaceSettlement(Coordinate location, Player& Owner) {
	if (resources.find(location) == resources.end() && !outOfBounds(location))
		corners[location] = std::unique_ptr<CornerPiece>(
				new Settlement(*this, location, Owner));

}

/**
 * Whether a settlement at a location can be upgraded to a city. 
 */
bool GameBoard::canUpgradeSettlement(Coordinate location,
		const Player& owner) const {
	auto it = corners.find(location);
	if (it == corners.end()) {
		std::cout << "there's nothing there" << std::endl;
		return false;
	}
	if (!it->second) {
		std::cout << "null ptr there" << std::endl;
		return false;
	}
	if (!(it->second->getOwner() == owner)) {
		std::cout << "wrong owner" << std::endl;
		return false;
	}
	if (dynamic_cast<const Settlement*>(it->second.get()) == 0) {
		std::cout << "this isn't a settlement" << std::endl;
		return false;
	}
	return true;
}

bool GameBoard::buyUpgradeOnSettlement(Coordinate location, Player& owner) {
	if (canUpgradeSettlement(location, owner) && owner.canBuyCity()) {
		if (!owner.buyCity()) {
			std::cout << "wat" << std::endl;
			return false;
		}
		UpgradeSettlement(location);
		return true;
	}
	std::cout << "failed for some reason" << std::endl;
	return false;
}

/**
 * Whether a settlement/city at a location can be upgraded to a wonder.
 */
bool GameBoard::canUpgradeToWonder(Coordinate location,
		const Player& owner) const {
	auto it = corners.find(location);
	if (it == corners.end()) {
		std::cout << "there's nothing there" << std::endl;
		return false;
	}
	if (!it->second) {
		std::cout << "null ptr there" << std::endl;
		return false;
	}
	if (!(it->second->getOwner() == owner)) {
		std::cout << "wrong owner" << std::endl;
		return false;
	}
	if (dynamic_cast<const Settlement*>(it->second.get()) == 0
			&& dynamic_cast<const City*>(it->second.get()) == 0) {
		std::cout << "this isn't a settlement or city" << std::endl;
		return false;
	}
	return true;
}

bool GameBoard::buyUpgradeOnWonder(Coordinate location, Player& owner) {
	if (canUpgradeToWonder(location, owner) && owner.canBuyCity()) {
		if (!owner.buyWonder()) {
			std::cout << "wat" << std::endl;
			return false;
		}
		UpgradeToWonder(location);
		return true;
	}
	std::cout << "failed for some reason" << std::endl;
	return false;
}

/**
 * Place a city on the board.
 * @param location Where to place it on the board.
 * @param Owner The player placing the city.
 */
void GameBoard::PlaceCity(Coordinate location, Player& Owner) {
	corners[location] = std::unique_ptr<CornerPiece>(
			new City(*this, location, Owner));

}

/**
 * Place a Wonder on the board.
 * @param location Where to place it on the board.
 * @param Owner The player placing the city.
 */
void GameBoard::PlaceWonder(Coordinate location, Player& Owner) {
	corners[location] = std::unique_ptr<CornerPiece>(
			new Wonder(*this, location, Owner));

}

/**
 * Upgrade a settlement to a city.
 * @param location Where the settlement being upgraded is.
 */
void GameBoard::UpgradeSettlement(Coordinate location) {
	if (corners.find(location) != corners.end())
		corners[location] = std::unique_ptr<CornerPiece>(
				new City(*corners[location])); //TODO test for memory leak
}

/**
 * Upgrade a settlement to a wonder.
 * @param location Where the settlement being upgraded is.
 */
void GameBoard::UpgradeToWonder(Coordinate location) {
	if (corners.find(location) != corners.end())
		corners[location] = std::unique_ptr<CornerPiece>(
				new Wonder(*corners[location])); //TODO test for memory leak
}

/**
 * Accept a visitor and have it visit all the game pieces, players, and roads.
 * @param visitor The instance visiting this GameBoard.
 */
void GameBoard::accept(GameVisitor& visitor) {
	visitor.visit(*this);

	// Drawing needs this to happen in this order. Visitors technically should be order-independent, but this was an easy fix at the moment.
	// Keep that in mind when modifying this.
	for (auto& it : resources) {
		it.second->accept(visitor);
	}
	for (auto& it : corners) {
		it.second->accept(visitor);

	}
	for (auto& roadCoordVec : roads) {
		for (auto& road : roadCoordVec.second) {
			if (road.get()) {
				road->accept(visitor);
			}
		}
	}
	for (auto& it : players) {
		if (it.get()) {
			it->accept(visitor);
		}
	}
	dice.accept(visitor);
}

/**
 * Equality comparison with another GameBoard.
 * @param other The board to compare with this GameBoard.
 * @return Whether the two boards are equal.
 */
bool GameBoard::operator==(const GameBoard& other) const {
	for (auto& it : corners) {
		auto otherIt = other.corners.find(it.first);
		if (otherIt == other.corners.end()) {
			if (it.second.get()) {
				return false; // This location isn't in the other array
			}
		} else if (!(*(it.second) == *(otherIt->second))) {
			return false;
		}
	}
	for (auto& it : resources) {
		auto otherIt = other.resources.find(it.first);
		if (otherIt == other.resources.end()) {
			return false; // This location isn't in the other array
		}
		if (!(*(it.second) == *(otherIt->second))) {
			return false;
		}
	}
	for (auto& roadCoordVec : roads) {
		const auto& otherVecIt = other.roads.find(roadCoordVec.first);
		if (otherVecIt == other.roads.end()) {
			return false;
		}
		auto& otherCoordVec = *otherVecIt;
		if (roadCoordVec.second.size() != otherCoordVec.second.size()) {
			return false;
		}
		for (size_t i = 0; i < roadCoordVec.second.size(); i++) {
			const Road& myRoad = *(roadCoordVec.second[i]);
			const Road& otherRoad = *(otherCoordVec.second[i]);
			if (myRoad == otherRoad) {
			} else {
				return false;
			}
		}
	}
	if (players.size() != other.players.size()) {
		return false;
	}
	for (unsigned int i = 0; i < players.size(); i++) {
		if (!(*(players[i]) == *(other.players[i]))) {
			return false;
		}
	}
	for (auto& it : corners) {
		auto otherIt = other.corners.find(it.first);
		if (otherIt == other.corners.end()) {
			return false;
		} else if (!(*it.second == *otherIt->second)) {
			return false;
		}
	}
	return true;
}
/**
 *  Adds a resource and roll tile combo to the board
 *
 *  @param x The first coordinate
 *  @param y The second coordinate
 *  @param res The resource type to be added
 *  @param val The roll tile to be added
 */
void GameBoard::addResource(int x, int y, resourceType res, int val) {
	this->resources[Coordinate(x, y)] = std::unique_ptr<ResourceTile>(
			new ResourceTile(*this, Coordinate(x, y), res, val));
}

bool GameBoard::isValidBoard() const {
	for (auto& resourcePair : resources) {
		const ResourceTile& tile =
				dynamic_cast<const ResourceTile&>(*resourcePair.second);

		if (tile.getDiceValue() != 6 && tile.getDiceValue() != 8) {
			continue;
		}

		static const Coordinate adjacentTileOffsets[] = { Coordinate(1, 1),
				Coordinate(2, -1), Coordinate(1, -2), Coordinate(-1, -1),
				Coordinate(-2, 1), Coordinate(2, -1) };
		for (const Coordinate& offset : adjacentTileOffsets) {
			Coordinate adjacentTileCoord(
					tile.getLocation().first + offset.first,
					tile.getLocation().second + offset.second);
			auto otherCoordinateIt = resources.find(adjacentTileCoord);
			if (otherCoordinateIt != resources.end()
					&& otherCoordinateIt->second) {
				const ResourceTile& otherTile =
						dynamic_cast<const ResourceTile&>(*otherCoordinateIt->second);
				if (otherTile.getDiceValue() == 6
						|| otherTile.getDiceValue() == 8) {
					return false;
				}
			}
		}
	}
	return true;
}

/**
 * The players in the game.
 * @return The players.
 */
const std::vector<std::unique_ptr<Player>>& GameBoard::getPlayers() const {
	return players;
}

/**
 * @return reference to the current Player
 */
Player& GameBoard::getCurrentPlayer() const {
	return *players[currentTurn];
}

/**
 * @return true if game has a winner, false otherwise
 */
bool GameBoard::hasWinner() {
	if (winner == -1)
		return false;
	return true;
}

/**
 * @return reference to the winner if there is one, null otherwise
 */
Player& GameBoard::getWinner() const {
	if (winner != -1 && winner < players.size())
		return *players[winner];

	return *players[0];
}

/**
 * @return no of players
 */
int GameBoard::getNoOfPlayers() {
	return players.size();
}

/**
 * @param index The index to look at.
 * @return player at index index
 */
Player& GameBoard::getPlayer(int index) {
	if (index >= getNoOfPlayers())
		throw std::runtime_error("Invalid player index.");

	return *players[index];
}

/**
 *  When a player begins their turn, this rolls the dice and takes the required action (paying resources or enabling robber movement)
 *  @return A pair of the values of the dice.
 */
std::pair<int, int> GameBoard::startTurn() {
	int roll = dice.roll();
	if (roll == 7) {
		enableRobber();
	} else {
		payoutResources(roll);
	}

	return std::make_pair(dice.getFirst(), dice.getSecond());
}

/**
 *  When a 7 is rolled, this enforces resource discarding and allows the current player to move the robber
 */
void GameBoard::enableRobber() {
	//Do some straight up robber stuff.
}

/**
 * This pays resources based on the current roll
 * @param roll This turn's dice roll.
 */
void GameBoard::payoutResources(int roll) {
	for (auto& it : resources) {
		if (it.second->getDiceValue() == roll) {
			it.second->Payout();
		}
	}
}

/**
 * Buys a card drawn from the deck
 */
void GameBoard::buyCard(Player& owner) {
	if (owner.canBuyCard() && deck.getSize() > 0) {

		DevelopmentCard * card_ptr = deck.drawCard();

		std::unique_ptr<DevelopmentCard> knight = std::unique_ptr<
				DevelopmentCard>(new KnightCard());
		std::unique_ptr<DevelopmentCard> victorypoint = std::unique_ptr<
				DevelopmentCard>(new VictoryPointCard());
		std::unique_ptr<DevelopmentCard> monopoly = std::unique_ptr<
				DevelopmentCard>(new MonopolyCard());
		std::unique_ptr<DevelopmentCard> yearofplenty = std::unique_ptr<
				DevelopmentCard>(new YearOfPlentyCard());
		std::unique_ptr<DevelopmentCard> roadbuilding = std::unique_ptr<
				DevelopmentCard>(new RoadBuildingCard());

		switch (card_ptr->getType()) {
		case KNIGHT:
			owner.buyCard(knight);
			break;
		case VICTORYPOINT:
			owner.buyCard(victorypoint);
			break;
		case MONOPOLY:
			owner.buyCard(monopoly);
			break;
		case YEAROFPLENTY:
			owner.buyCard(yearofplenty);
			break;
		case ROADBUILDING:
			owner.buyCard(roadbuilding);
			break;
		default:
			break;
		}

		delete (card_ptr);
	}
}

/**
 * Discards a card back into the deck
 */
void GameBoard::discardCard(DevelopmentCard * card) {
	deck.discard(card);
}

