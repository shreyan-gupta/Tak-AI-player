#include "Header.h"
using namespace Types;

void search(bool type, int x, int y, bool player, vector< vector<Position> > &GameBoard, vector< vector<bool> > &explored, bool &found, int size){
	if(found || explored[x][y] || x<0 || x==size || y<0 || y==size) return;
	if(type && y == size-1){
		found = true;
		return;
	}
	if(!type && x == size-1){
		found = true;
		return;
	}
	explored[x][y] = true;
	if(!GameBoard[x][y+1].empty() && GameBoard[x][y+1].top_piece().second == player) 
		search(type, x, y+1, player, GameBoard, explored, found, size);
	if(!GameBoard[x+1][y].empty() && GameBoard[x+1][y].top_piece().second == player) 
		search(type, x+1, y, player, GameBoard, explored, found, size);
	if(!GameBoard[x-1][y].empty() && GameBoard[x-1][y].top_piece().second == player) 
		search(type, x-1, y, player, GameBoard, explored, found, size);
	if(!GameBoard[x][y-1].empty() && GameBoard[x][y-1].top_piece().second == player) 
		search(type, x, y-1, player, GameBoard, explored, found, size);
}

eval_type Game::feature0(){
	vector< vector<bool> > explored(size, vector<bool>(size, false));
	for(int i=0; i<size; ++i){
		if(GameBoard[i][0].empty() || explored[i][0]) continue;
		bool found = false;
		int x = i;
		int y = 0;
		search(true,x,y,GameBoard[i][0].top_piece().second, GameBoard, explored, found, size);
		if(found){
			if(GameBoard[i][0].top_piece().second == White) return 1;
			else return -1;
		}
	}
	for(int j=0; j<size; ++j){
		if(GameBoard[0][j].empty() || explored[0][j]) continue;
		bool found = false;
		int x = 0;
		int y = j;
		search(false,x,y,GameBoard[0][j].top_piece().second, GameBoard, explored, found, size);
		if(found){
			if(GameBoard[0][j].top_piece().second == White) return 1;
			else return -1;
		}
	}
	return 0;
}

// Number of white and black on board
eval_type Game::feature1(){
	int count = 0;
	for(int i=0; i<size; ++i){
		for(int j=0; j<size; ++j){
			if(GameBoard[i][j].empty()) continue;
			if(GameBoard[i][j].top_piece().second == White) ++count;
			else --count;
		}
	}
	return count;
}

eval_type Game::feature2(){
	return 0;
}

eval_type Game::feature3(){
	return 0;
}