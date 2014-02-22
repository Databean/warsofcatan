#include "GameBoard.h"

GameBoard::GameBoard() {
	
}

GameBoard::~GameBoard() {
	
}

int GameBoard::saveBoardToFile(std::string filename){
	std::ofstream file;
	file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try {
		filename = filename + ".wocs";
		file.open(filename.c_str(), std::fstream::out | std::fstream::trunc);
		constructFileFromBoard(file);

	} catch (std::ofstream::failure e){
		std::cerr << "Exception opening/reading/closing file \n";
	}



	return 0;
}

int GameBoard::loadBoardFromFile(std::string filename){
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		filename = filename + ".wocs";
		file.open(filename.c_str(), std::fstream::in);
		constructBoardFromFile(file);

		file.close();
	} catch (std::ifstream::failure e) {
		std::cerr << "Exception opening/reading/closing file\n";
	}
	return 0;
}

int GameBoard::constructBoardFromFile(std::ifstream &file){
	//Parse and construct the board from the file
	//@ TODO
	while (!file.eof()) {
		std::cout << file.get();
	}
	return 0;
}


int GameBoard::constructFileFromBoard(std::ofstream &file){
	//Construct the file based on the structure of the board
	//@ TODO
	file << "Hello World!";
	return 0;
}












