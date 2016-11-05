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

inline eval_type Game::center(int i, int j){
	return -(abs(i - size/2.0) + abs(j - (size/2.0))) / size;
}

inline eval_type Game::piece_type(char top){
	switch(top){
		case 'F' : return w[x]; 
		case 'S' : return w[x];
		default : return w[x];
	}
}

inline eval_type Game::captive(char top, pair<s_int, s_int> &p){
	switch(top){
		case 'F' : return p.first * w[x] + p.second * w[x]; 
		case 'S' : return p.first * w[x] + p.second * w[x];
		default :  return p.first * w[x] + p.second * w[x];
	}
}

eval_type Game::features(){
	eval_type count = 0;
	pair<s_int, s_int> p;
	for(s_int i=0; i<size; ++i){
		for(s_int j=0; j<size; ++j){
			Position &pos = GameBoard[i][j];
			if(pos.empty()) continue;
			int mult = pos.player() ? 1 : -1;
			eval_type temp_count = 0;
			pos.captive(p);

			temp_count += center(i,j) * w[x];
			temp_count += piece_type(pos.top_piece());
			temp_count += captive(pos.top_piece(), p);

			count += mult * temp_count;
		}
	}
}












