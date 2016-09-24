#include "Header.h"

Board::Board(int size){
	GameBoard = vector< vector<Position> >(size, vector<Position>(size));
}

