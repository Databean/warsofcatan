#include "GameBoard.h"

#include <map>
#include <memory>

using std::map;
using std::unique_ptr;

#include <ctime>
#include <algorithm>

using std::random_shuffle;
using std::time;

GameBoard::GameBoard() {
	init_resources();
}

GameBoard::~GameBoard() {
	
}


int GameBoard::save_Board(std::string filename){
	std::ofstream file;
	file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try {
		filename = filename + ".wocs";
		file.open(filename.c_str());
		constructFileFromBoard(file);
		file.close();
		return 0;
	} catch (std::ofstream::failure e) {
		std::cerr << "Exception opening/closing/writing file: " << e.what();
	}
	return -1;
}

int GameBoard::load_Board(std::string filename){
	std::ifstream file;
	try {
		filename = filename + ".wocs";
		file.open(filename.c_str());
		constructBoardFromFile(file);
		file.close();
		return 0;
	} catch (std::ifstream::failure e) {
		std::cerr << "Exception opening/closing/reading file: " << e.what();
	}
	return -1;
}

int GameBoard::constructBoardFromFile(std::ifstream &file){
	//Parse and construct the board from the file
	//@ TODO
	std::string line;
	if (file.is_open()) {
		while (getline(file, line)) {
			std::cout << line << '\n';
		}
	}
	return 0;
}


int GameBoard::constructFileFromBoard(std::ofstream &file){
	//Construct the file based on the structure of the board
	//@ TODO
	file << "Hello World!";
	return 0;
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
    
    int rolls[] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11};
    while !checkRolls(rolls)
    {
        random_shuffle(&rolls[0], &rolls[18]);
    }
    
    int xcoords[] = {-2, 0, 2, -3, -1, 1, 3, -4, -2, 0, 2, 4, -3, -1, 1, 3, -2, 0, 2};
    int ycoords[] = { 2, 1, 0,  4,  3, 2, 1,  6, 5,  4, 3, 2,  7,  6, 5, 4,  8, 7, 6};
	
    
    
    int rollCount = 0;
    for (int i = 0; i<19; i++)
    {
        if (resources[i]==DESERT)
        {
            addResource(xcoords[i], ycoords[i], resources[i], 0);
        }
        else
        {
            addResource(xcoords[i], ycoords[i], resources[i], rolls[rollCount]);
            rollCount++;
        }
    }
    
}

void GameBoard::PlaceSettlement(Coordinate location, Player& Owner){
	corners[location] = std::unique_ptr<GamePiece>(new Settlement(*this, location, Owner));
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
