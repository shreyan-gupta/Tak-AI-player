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

void Game::newsearch(s_int x, s_int y, bool player, vector< vector<bool> > &explored, vector<int> &lrud){
	if(explored[x][y] || x<0 || x==size || y<0 || y==size) return;
	lrud[0] = min(lrud[0], x);
	lrud[1] = max(lrud[1], x);
	lrud[2] = min(lrud[2], y);
	lrud[3] = max(lrud[3], y);
	explored[x][y] = true;

	if(y!=size-1 && pathable(x,y+1,player))
		newsearch(x, y+1, player, explored, lrud);
	if(x!=size-1 && pathable(x+1,y,player)) 
		newsearch(x+1, y, player, explored, lrud);
	if(x!=0 && pathable(x-1,y,player)) 
		newsearch(x-1, y, player, explored, lrud);
	if(y!=0 && pathable(x,y-1,player)) 
		newsearch(x, y-1, player, explored, lrud);
}

eval_type Game::newpath()
{
	vector< vector<bool> > explored(size, vector<bool>(size, false));
	eval_type GroupVal = 0;
	for (s_int i = 0; i < size; i++)
		for (s_int j = 0; j < size; j++)
		{
			if (GameBoard[i][j].empty() || GameBoard[i][j].top_piece() == 'S' || explored[i][j]) continue;
			vector<int> LRUD (4);
			LRUD[0] = LRUD[1] = i;
			LRUD[2] = LRUD[3] = j;
			newsearch(i, j, GameBoard[i][j].player(), explored, LRUD);
			GroupVal += (GROUP[LRUD[1] - LRUD[0]] + GROUP[LRUD[3] - LRUD[2]])*(GameBoard[i][j].player() == White ? 1 : -1);
			// fprintf(stderr, "%d = i,%d = j, %d %d %d %d is LRUD. \n", i, j, LRUD[0], LRUD[1], LRUD[2], LRUD[3]);
		}
	return GroupVal;
}

eval_type Game::path(){
	vector< vector<bool> > explored(size, vector<bool>(size, false));
	// vector<int> LRUD (4,0);
	for(s_int i=0; i<size; ++i){
		if(GameBoard[i][0].empty() || GameBoard[i][0].top_piece() == 'S' || explored[i][0]) continue;
		bool found = false;
		s_int x = i;
		s_int y = 0;
		search(true, x, y, GameBoard[i][0].player(), explored, found);
		if(found){
			if(GameBoard[i][0].player() == White) return RDWIN;
			else return -RDWIN;
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
			if(GameBoard[0][j].player() == White) return RDWIN;
			else return -RDWIN;
		}
	}
	return 0;
}

inline eval_type Game::center(int i, int j){
	return -(abs(2*i - size) + abs(2*j - (size))) * CENTER;
}

inline eval_type Game::piece_type(char top){
	switch(top){
		case 'F' : return TOPFLAT + ENDGAMEFLAT * (eval_type)(ENDGAMECUTOFF - min((s_int)min(p_black.StonesLeft, p_white.StonesLeft), (s_int)ENDGAMECUTOFF)) / ENDGAMECUTOFF; 
		case 'S' : return STAND;
		default : return CAP;
	}
}

inline eval_type Game::captive(char top, pair<s_int, s_int> &p){
	switch(top){
		case 'F' : return p.first * HARD_FCAPTIVE + p.second * SOFT_FCAPTIVE; 
		case 'S' : return p.first * HARD_SCAPTIVE + p.second * SOFT_SCAPTIVE;
		default :  return p.first * HARD_CCAPTIVE + p.second * SOFT_CCAPTIVE;
	}
}

eval_type Game::features(){
	eval_type count = 0;
	pair<s_int, s_int> p;
	bool has_empty = false;
	int delta_flat = 0;
	for(s_int i=0; i<size; ++i){
		for(s_int j=0; j<size; ++j){
			Position &pos = GameBoard[i][j];
			if(pos.empty()){
				has_empty = true;
				continue;
			}
			if 	(pos.top_piece() != 'S'){
				if(pos.player() == White) ++delta_flat;
				else --delta_flat;
			}
			int mult = pos.player() ? 1 : -1;
			eval_type temp_count = 0;
			pos.captive(p);

			temp_count += center(i,j);
			temp_count += piece_type(pos.top_piece());
			temp_count += captive(pos.top_piece(), p);

			// Groups : **************************
			vector<s_int> stack_xy (4,0);
			if (pos.player() == White)
			{
				GetStackable(i,j,White,stack_xy);
				// temp_count += GroupWt[max(stack_xy[0],stack_xy[1])] + GroupWt[max(stack_xy[2],stack_xy[3])];				
			}
			else
			{
				GetStackable(i,j,Black,stack_xy);
				// temp_count -= GroupWt[max(stack_xy[0],stack_xy[1])] + GroupWt[max(stack_xy[2],stack_xy[3])];				
			}


			count += mult * temp_count;
			// cout << count << " is the count, inside features \n";
		}
	}
	// cout << "-------------------- \n";
	if(has_empty == false || p_black.noStone() || p_white.noStone()){
		if(delta_flat > 0) return FLWIN;
		else if(delta_flat == 0) return count;
		else return -FLWIN;
		// return (delta_flat > 0) ? FLWIN : -FLWIN;
	}
	else return count;
}












