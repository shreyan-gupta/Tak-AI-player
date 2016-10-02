#include "Header.h"
using namespace Types;

inline bool pathable(int x, int y, bool player, vector< vector<Position> > &GameBoard){
	return (!GameBoard[x][y].empty() && GameBoard[x][y].top_piece().first != Stand && GameBoard[x][y].top_piece().second == player);
}

void search(bool type, int x, int y, bool player, vector< vector<Position> > &GameBoard, vector< vector<bool> > &explored, bool &found, int size){
	// fprintf(stderr, "Search %d %d %d\n", x, y, found);
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
	if(y!=size-1 && pathable(x,y+1,player,GameBoard))
		search(type, x, y+1, player, GameBoard, explored, found, size);
	if(x!=size-1 && pathable(x+1,y,player,GameBoard)) 
		search(type, x+1, y, player, GameBoard, explored, found, size);
	if(x!=0 && pathable(x-1,y,player,GameBoard)) 
		search(type, x-1, y, player, GameBoard, explored, found, size);
	if(y!=0 && pathable(x,y-1,player,GameBoard)) 
		search(type, x, y-1, player, GameBoard, explored, found, size);
}

// int favourableStack(vector< vector<Position> > &Board, int i, int j)
// {
// 	Position &p = Board[i][j];
// 	Player_Type s_top = p.Stack.front().second;
// 	float mult = (s_top == White) ? 1 : -1;
// 	bool opp_Wall_around = false;
// 	bool opp_cap_around = false;
// 	bool opp_around = false;
// 	if (i+1 < Size)
// 	{
// 		if (!Board[i+1][j].empty() && Board[i+1][j].Stack.front().second != s_top)
// 		{
// 			opp_Wall_around = (Board[i+1][j].Stack.front().first == Stand);
// 			opp_around = true;
// 			opp_cap_around = (Board[i+1][j].Stack.front().first == Cap);
// 		}
// 	}
// 	if (i-1 >= 0)
// 	{
// 		if (!Board[i-1][j].empty() && Board[i-1][j].Stack.front().second != s_top)
// 		{
// 			opp_Wall_around = opp_Wall_around || (Board[i-1][j].Stack.front().first == Stand);
// 			opp_around = true;
// 			opp_cap_around = (Board[i-1][j].Stack.front().first == Cap);
// 		}		
// 	}
// 	if (j+1 < Size && !opp_Wall_around)
// 	{
// 		if (!Board[i][j+1].empty() && Board[i][j+1].Stack.front().second != s_top)
// 		{
// 			opp_Wall_around = opp_Wall_around || (Board[i][j+1].Stack.front().first == Stand);
// 			opp_around = true;
// 			opp_cap_around = (Board[i][j+1].Stack.front().first == Cap);
// 		}
// 	}
// 	if (j-1 >= 0 && !opp_Wall_around)
// 	{
// 		if (!Board[i][j-1].empty() && Board[i][j-1].Stack.front().second != s_top)
// 		{
// 			opp_Wall_around = opp_Wall_around || (Board[i][j-1].Stack.front().first == Stand);
// 			opp_around = true;
// 			opp_cap_around = (Board[i][j-1].Stack.front().first == Cap);
// 		}
// 	}

// 	float ans = 0.0;
// 	if (opp_around)
// 	{
// 		if (p.Stack.front().first == Flat)
// 		{
// 			if (opp_Wall_around) ans = 2;
// 			else ans = 5;
// 		}
// 		else if (p.Stack.front().first == Stand)
// 			ans = 6;
// 		else
// 			ans = 15;
// 	}
// 	else
// 		ans = 12;
// 	return mult*ans;
// }

// path
eval_type Game::feature0(){
	// fprintf(stderr, "Feature 0 enter \n");
	vector< vector<bool> > explored(size, vector<bool>(size, false));
	for(int i=0; i<size; ++i){
		if(GameBoard[i][0].empty() || explored[i][0]) continue;
		bool found = false;
		int x = i;
		int y = 0;
		search(true,x,y,GameBoard[i][0].top_piece().second, GameBoard, explored, found, size);
		if(found){
			if(GameBoard[i][0].top_piece().second == White) return w[0];
			else return -w[0];
		}
	}
	for(int j=0; j<size; ++j){
		if(GameBoard[0][j].empty() || explored[0][j]) continue;
		bool found = false;
		int x = 0;
		int y = j;
		search(false,x,y,GameBoard[0][j].top_piece().second, GameBoard, explored, found, size);
		if(found){
			if(GameBoard[0][j].top_piece().second == White) return w[0];
			else return -w[0];
		}
	}
	return 0;
}

// closer to center
eval_type Game::feature1(){
	// fprintf(stderr, "Feature 1 enter \n");
	float half = size/2.0;
	eval_type count = 0;
	for(int i=0; i<size; ++i){
		for(int j=0; j<size; ++j){
			if(GameBoard[i][j].empty()) continue;
			if(GameBoard[i][j].top_piece().second == White) count += (abs(i-half) + abs(j-half));
			else count -= (abs(i-half) + abs(j-half));
		}
	}
	return w[1]*count;
}

inline eval_type Game::nbr(Piece p1, Piece p2){
	Stone &s1 = p1.first;
	Stone &s2 = p2.first;
	bool same = (p1.second == p2.second);
	int mult = (p1.second == White) ? 1 : -1;
	if(same){
		if(s1 == Flat){
			switch(s2){
				case Flat 	: return mult * w[9];
				case Stand 	: return mult * w[10];
				case Cap	: return mult * w[11];
			}
		}
		else if(s1 == Stand){
			switch(s2){
				case Stand 	: return mult * w[12];
				case Cap 	: return mult * w[13];
				default 	: return 0;
			}
		}
	}
	else{
		if(s1 == Flat){
			switch(s2){
				case Stand 	: return mult * w[14];
				case Cap 	: return mult * w[2];
				default 	: return 0;
			}
		}
		else if(s1 == Stand){
			if(s2 == Cap) return mult * w[3];
		}
		return 0;
	}
}

// neighbours
eval_type Game::feature2(){
	// fprintf(stderr, "Feature 2 enter \n");
	eval_type count = 0;
	for(int x=0; x<size; ++x){
		for(int y=0; y<size; ++y){
			if(GameBoard[x][y].empty()) continue;
			if(x != size-1 && !GameBoard[x+1][y].empty())
				count += nbr(GameBoard[x][y].top_piece(), GameBoard[x+1][y].top_piece());

			if(y != size-1 && !GameBoard[x][y+1].empty())
				count += nbr(GameBoard[x][y].top_piece(), GameBoard[x][y+1].top_piece());
		}
	}
	return count;
}

// neighbour weights
// eval_type Game::feature2(){
// 	eval_type count = 0;
// 	if(p_white.CapLeft){
// 		int x = p_white.x;
// 		int y = p_white.y;
// 		if(y != size-1 && !GameBoard[x][y+1].empty() && GameBoard[x][y+1].top_piece().second == Black)
// 			count += cap_neighbour(GameBoard[x][y+1].top_piece().first);
// 		if(x != size-1 && !GameBoard[x+1][y].empty() && GameBoard[x+1][y].top_piece().second == Black)
// 			count += cap_neighbour(GameBoard[x+1][y].top_piece().first);
// 		if(y != 0 && !GameBoard[x][y-1].empty() && GameBoard[x][y-1].top_piece().second == Black)
// 			count += cap_neighbour(GameBoard[x][y-1].top_piece().first);
// 		if(x != 0 && !GameBoard[x-1][y].empty() && GameBoard[x-1][y].top_piece().second == Black)
// 			count += cap_neighbour(GameBoard[x-1][y].top_piece().first);
// 	}
// 	if(p_black.CapLeft){
// 		int x = p_black.x;
// 		int y = p_black.y;
// 		if(y != size-1 && !GameBoard[x][y+1].empty() && GameBoard[x][y+1].top_piece().second == White)
// 			count -= cap_neighbour(GameBoard[x][y+1].top_piece().first);
// 		if(x != size-1 && !GameBoard[x+1][y].empty() && GameBoard[x+1][y].top_piece().second == White)
// 			count -= cap_neighbour(GameBoard[x+1][y].top_piece().first);
// 		if(y != 0 && !GameBoard[x][y-1].empty() && GameBoard[x][y-1].top_piece().second == White)
// 			count -= cap_neighbour(GameBoard[x][y-1].top_piece().first);
// 		if(x != 0 && !GameBoard[x-1][y].empty() && GameBoard[x-1][y].top_piece().second == White)
// 			count -= cap_neighbour(GameBoard[x-1][y].top_piece().first);
// 	}
// 	return count;
// }

eval_type Game::stone_weight(Stone s){
	switch(s){
		case Flat : return w[6];
		case Stand : return w[7];
		default : return w[8];
	}
}

// How many of the same type below the stack
// Weight of each stack as per piece
eval_type Game::feature3(){
	// fprintf(stderr, "Feature 3 enter \n");
	eval_type count = 0;
	pair<int,int> top5;
	for(auto &i : GameBoard){
		for(auto &j : i){
			if(j.empty()) continue;
			j.top5(top5);
			if(j.top_piece().second == White){
				count += stone_weight(j.top_piece().first);
				count += ((top5.first)*w[4] + (top5.second)*w[5]);
			}
			else{
				count -= stone_weight(j.top_piece().first);
				count -= ((top5.first)*w[5] + (top5.second)*w[4]);
			}
		}
	}
	return count;
}

eval_type Game::feature4()
{
	eval_type count = 0;
	if ((p_black.StonesLeft + (int)p_black.CapLeft) == 0 || (p_white.StonesLeft + (int)p_white.CapLeft) == 0)
	{
		// who won??
		for (int i = 0 ; i < size ; i ++)
		{
			for (int j = 0 ; j < size ; j ++)
			{
				if (GameBoard[i][j].empty()) continue;
				if (GameBoard[i][j].top_piece().second == White) count ++;
				else count --;
			}
		}
	}
	return count*w[15];
}


eval_type Game::feature5(){
	vector<vector<int> > influence (Size, vector<int> (Size,0));
	int mult = 0;
	eval_type count = 0;
	for (int i = 0 ; i < Size ; i ++)
	{
		for (int j = 0 ; j < Size ; j ++)
		{
			if (GameBoard[i][j].empty()) continue;
			mult = (GameBoard[i][j].top_piece().second == White) ? 1 : -1;
			influence[i][j] += mult;
			if (i > 0)
				influence[i-1][j] += mult;
			if (i < size-1)
				influence[i+1][j] += mult;
			if (j > 0)
				influence[i][j-1] += mult;
			if (j < size-1)
				influence[i][j+1] += mult;
		}
	}
	// counted all!!
	for (int i = 0 ; i < size ; i ++)
	{
		for (int j = 0 ; j < size ; j ++)
			count += influence[i][j];
	}
	return count*w[16];
}

// Number of white and black on board
// eval_type Game::feature4(){
// 	float count = 0;
// 	for(int i=0; i<size; ++i){
// 		for(int j=0; j<size; ++j){
// 			if(GameBoard[i][j].empty()) continue;
// 			count += favourableStack(GameBoard, i, j);
// 		}
// 	}
// 	return count;
// }
