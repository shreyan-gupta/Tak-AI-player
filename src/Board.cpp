#include "Header.h"
using namespace Types;

inline Piece Position::top_piece(){
	return Stack.front();
}

inline bool Position::empty(){
	return Stack.empty();
}

string Position::to_string(){
	int val=0;
	int mul = 1;
	string s = "";
	for(auto &i : Stack){
		if(i.second == Black) val += mul;
		mul = mul << 1;
	}
	val += mul;
	s += std::to_string(val);
	if(!Stack.empty()){
		if(Stack.back().first == F) s += "!";
		else if(Stack.back().first == S) s+= "@";
		else s += "#";
	}
	return s;
}

Game::Game(int size){
	this->size = size;
	GameBoard = vector< vector<Position> >(size, vector<Position>(size));
}

Game::~Game()
{
	// do nothing!
}

string Game::to_string()
{
	string str = "";
	for (int i = 0 ; i < size ; i ++)
	{
		for (int j = 0 ; j < size ; j ++)
		{
			str += GameBoard[i][j].to_string() + "_"; // this is a position.
		}
	}
	return str;
}

eval_type Game::eval()
{
	// TODO
}

std::pair<Move,ll> Game::decide_move(int depth, bool max)
{
	std::multimap<ll,Move> allmoves;
	generate_valid_moves(Max,allmoves);
	// iterate over allmoves.
	for (auto &i : allmoves)
	{
		// makemove.
		// ask p2 to decide his best move
		// make anti move.
		makemove(i.first);

	}
}


void Game::makemove(Move m)
{
	// TODO
	if (m.Place_Move)
	{
		// x,y pe posn mein push kardo. (stack must be empty abhi.)
		if (m.Type)
		{
			// push WHAT?? 
			GameBoard[m.x][m.y].Stack.push_back(m.p);
			if (m.p.Player_Type == Black)
				GameBoard[m.x][m.y].Num_Black += 1;
			else
				GameBoard[m.x][m.y].Num_White += 1;
		}
		else
		{
			// POP!
			GameBoard[m.x][m.y].Stack.pop_back();
			if (m.p.Player_Type == Black)
				GameBoard[m.x][m.y].Num_Black -= 1;
			else
				GameBoard[m.x][m.y].Num_White -= 1;
		}
	}
	else
	{
		// move/anti?
		vector<int> & d = m.Drops;
		char dirn = m.Direction;
		int x_add = (dirn == '>') ? 1 : ((dirn == '<') ? -1 : 0);
		int y_add = (dirn == '+') ? 1 : ((dirn == '-') ? -1 : 0);
		if (m.Type)
		{
			int drop_x = m.x + x_add;
			int drop_y = m.y + y_add;

			std::deque<Piece> & mainstack = GameBoard[m.x][m.y].Stack;

			for (int i = 0 ; i < d.size() ; i ++)
			{
				int num_drops = d[i];
				for (int j = num_drops - 1 ; j > -1 ; j --)
				{
					Piece jth = mainstack[mainstack.size() - 1 - j];
					GameBoard[drop_x][drop_y].Stack.push_back(jth);
					if (jth.second == Black)
						GameBoard[drop_x][drop_y].Num_Black += 1;
					else
						GameBoard[drop_x][drop_y].Num_White += 1;
				}
				// pop all these!
				for (int j = 0 ; j < num_drops ; j ++)
				{
					if (mainstack.front().second == Black)
						mainstack.Num_Black -= 1;
					else
						mainstack.Num_White -= 1;
					mainstack.pop_back();
				}
				drop_x += x_add;
				drop_y += y_add;
			}
		}

		else
		{
			// ANTIMOVE!
			int pick_x = ;
			int pick_y = ;
		}
	}
}

void Game::generate_valid_moves(bool max, std::multimap<ll,Move> &moves)
{
	// TODO
	for (int i = 0 ; i < size ; i ++)
	{
		for (int j = 0 ; j < size ; j ++)
		{
			// empty? or stack?
			
		}
	}
}