#include "Game.h"
using namespace Types;

void Game::search(bool type, s_int x, s_int y, bool player, vector< vector<bool> > &explored, bool &found){
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
	if(y!=size-1 && pathable(x,y+1,player))
		search(type, x, y+1, player, explored, found);
	if(x!=size-1 && pathable(x+1,y,player)) 
		search(type, x+1, y, player, explored, found);
	if(x!=0 && pathable(x-1,y,player)) 
		search(type, x-1, y, player, explored, found);
	if(y!=0 && pathable(x,y-1,player)) 
		search(type, x, y-1, player, explored, found);
}

eval_type Game::path(){
	vector< vector<bool> > explored(size, vector<bool>(size, false));
	for(s_int i=0; i<size; ++i){
		if(GameBoard[i][0].empty() || GameBoard[i][0].top_piece() == 'S' || explored[i][0]) continue;
		bool found = false;
		s_int x = i;
		s_int y = 0;
		search(true, x, y, GameBoard[i][0].player(), explored, found);
		if(found){
			if(GameBoard[i][0].player() == White) return w[0];
			else return -w[0];
		}
	}
	explored = vector< vector<bool> >(size, vector<bool>(size, false));
	for(s_int j=0; j<size; ++j){
		if(GameBoard[0][j].empty() || GameBoard[0][j].top_piece() == 'S' || explored[0][j]) continue;
		bool found = false;
		s_int x = 0;
		s_int y = j;
		search(false, x, y, GameBoard[0][j].player(), explored, found);
		if(found){
			if(GameBoard[0][j].player() == White) return w[0];
			else return -w[0];
		}
	}
	return 0;
}