#include "GameBoard.h"

GameBoard::GameBoard() {
	
}

GameBoard::~GameBoard() {
	
}


int GameBoard::save_Board(std::string filename){
	std::ofstream * file = new std::ofstream;
	file->exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try {
		filename = filename + ".wocs";
		file->open(filename.c_str());
		constructFileFromBoard(file);
		file->close();
		delete(file);
		return 0;
	} catch (std::ofstream::failure e) {
		std::cerr << "Exception opening/closing/writing file: " << e.what();
		delete (file);
	}
	return -1;

}

int GameBoard::load_Board(std::string filename){
	std::ifstream * file = new std::ifstream;
	try {
		filename = filename + ".wocs";
		file->open(filename.c_str());
		constructBoardFromFile(file);
		file->close();
		delete(file);
		return 0;
	} catch (std::ifstream::failure e) {
		std::cerr << "Exception opening/closing/reading file: " << e.what();
		delete (file);
	}
	return -1;
}

int GameBoard::constructBoardFromFile(std::ifstream * file){
	//Parse and construct the board from the file
	//@ TODO
	std::string line;
	if (file->is_open()) {
		while (getline(*file, line)) {
			std::cout << line << '\n';
		}
	}
	return 0;
}


int GameBoard::constructFileFromBoard(std::ofstream * file){
	//Construct the file based on the structure of the board
	//@ TODO
	*file << "Hello World!";
	return 0;
}












