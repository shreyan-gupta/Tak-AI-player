#include "Header.h"
using namespace Types;

void search(bool type, int x, int y, bool player, vector< vector<Position> > &GameBoard, vector< vector<bool> > &explored, bool &found, int size){
	// printf("Search %d %d %d\n", x, y, found);
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
	if(y!=size-1 && !GameBoard[x][y+1].empty() && GameBoard[x][y+1].top_piece().second == player) 
		search(type, x, y+1, player, GameBoard, explored, found, size);
	if(x!=size-1 && !GameBoard[x+1][y].empty() && GameBoard[x+1][y].top_piece().second == player) 
		search(type, x+1, y, player, GameBoard, explored, found, size);
	if(x!=0 && !GameBoard[x-1][y].empty() && GameBoard[x-1][y].top_piece().second == player) 
		search(type, x-1, y, player, GameBoard, explored, found, size);
	if(y!=0 && !GameBoard[x][y-1].empty() && GameBoard[x][y-1].top_piece().second == player) 
		search(type, x, y-1, player, GameBoard, explored, found, size);
}

float favourableStack(vector< vector<Position> > &Board, int i, int j)
{
	Position &p = Board[i][j];
	Player_Type s_top = p.Stack.front().second;
	float mult = (s_top == White) ? 1 : -1;
	bool opp_Wall_around = false;
	bool opp_around = false;
	if (i+1 < Size)
	{
		if (!Board[i+1][j].empty() && Board[i+1][j].Stack.front().second != s_top)
		{
			opp_Wall_around = (Board[i+1][j].Stack.front().first == Stand);
			opp_around = true;
		}
	}
	if (i-1 >= 0)
	{
		if (!Board[i-1][j].empty() && Board[i-1][j].Stack.front().second != s_top)
		{
			opp_Wall_around = opp_Wall_around || (Board[i-1][j].Stack.front().first == Stand);
			opp_around = true;
		}		
	}
	if (j+1 < Size && !opp_Wall_around)
	{
		if (!Board[i][j+1].empty() && Board[i][j+1].Stack.front().second != s_top)
		{
			opp_Wall_around = opp_Wall_around || (Board[i][j+1].Stack.front().first == Stand);
			opp_around = true;
		}
	}
	if (j-1 >= 0 && !opp_Wall_around)
	{
		if (!Board[i][j-1].empty() && Board[i][j-1].Stack.front().second != s_top)
		{
			opp_Wall_around = opp_Wall_around || (Board[i][j-1].Stack.front().first == Stand);
			opp_around = true;
		}
	}

	float ans = 0.0;
	if (opp_around)
	{
		if (p.Stack.front().first != Stand)
		{
			if (opp_Wall_around) ans = 2;
			else ans = 5;
		}
		else
			ans = 7;
	}
	else
		ans = 10;
	return mult*ans;
}

int Game::feature0(){
	// cout << this->to_string() << "\n";
	vector< vector<bool> > explored(size, vector<bool>(size, false));
	for(int i=0; i<size; ++i){
		if(GameBoard[i][0].empty() || explored[i][0]) continue;
		bool found = false;
		int x = i;
		int y = 0;
		// cout << "Piece " << GameBoard[i][0].top_piece().second << " " << x << y << endl;
		search(true,x,y,GameBoard[i][0].top_piece().second, GameBoard, explored, found, size);
		// printf("FOUND %d\n", found);
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
		// cout << "Piece " << GameBoard[0][j].top_piece().second << " " << x << y << endl;
		search(false,x,y,GameBoard[0][j].top_piece().second, GameBoard, explored, found, size);
		// printf("FOUND %d\n", found);
		if(found){
			if(GameBoard[0][j].top_piece().second == White) return 1;
			else return -1;
		}
	}
	return 0;
}

// Number of white and black on board
int Game::feature1(){
	float count = 0;
	for(int i=0; i<size; ++i){
		for(int j=0; j<size; ++j){
			if(GameBoard[i][j].empty()) continue;
			// printf("Not Empty %d %d %d\n",i,j,GameBoard[i][j].empty());
			count += favourableStack(GameBoard, i, j);
			// ADD MORE IF NO WALL/CAP?? TODOOOO
		}
	}
	return count;
}

// How many of the same type below the stack
int Game::feature2(){
	int count = 0;
	pair<int,int> top5;
	for(auto &i : GameBoard){
		for(auto &j : i){
			if(j.empty()) continue;
			j.top5(top5);
			if(j.top_piece().second == White) count += top5.first;
			else count -= top5.second;
		}
	}
	return count;
}

int Game::feature3()
{
	// your no of walls!
	return 0;
}