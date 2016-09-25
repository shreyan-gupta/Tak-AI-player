#include "Header.h"
using namespace Types;

Player::Player(bool player_type, int pieces, int caps){
	type = player_type;
	StonesLeft = pieces;
	CapsLeft = caps;
}

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
	if(!empty()){
		if(Stack.back().first == Flat) s += "F";
		else if(Stack.back().first == Stand) s+= "S";
		else s += "C";
	}
	return s;
}

Game::Game(int size){
	this->size = size;
	GameBoard = vector< vector<Position> >(size, vector<Position>(size));
	p1 = Player(White, 100, 1);
	p1 = Player(Black, 100, 1);
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

std::pair<Move,eval_type> Game::decide_move(int depth, bool max)
{
	std::multimap<eval_type,Move> allmoves;
	generate_valid_moves(max,allmoves);
	// iterate over allmoves.
	for (auto &i : allmoves)
	{
		// makemove.
		// ask p2 to decide his best move
		// make anti move.
		makemove(i.second);

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
	}
}

void Game::generate_valid_moves(bool max, std::multimap<eval_type,Move> &moves)
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