#include "Header.h"

typedef std::pair<Stone,Player_Type> Piece;
typedef long double ll;

inline Piece Position::get_piece(){
	if(!Stack.empty()) return Stack.front();
	else return make_pair(F,None);
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
			str += GameBoard[i][j].to_string() + " "; // this is a position.
		}
	}
	return str;
}

ll Game::eval()
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
			if (p.Player_Type == Black)
				GameBoard[m.x][m.y].Num_Black += 1;
			else
				GameBoard[m.x][m.y].Num_White += 1;
		}
		else
		{
			// POP!
			GameBoard[m.x][m.y].Stack.pop_back();
			if (p.Player_Type == Black)
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

void Game::generate_valid_moves(bool max, std::multimap<ll,Move> & moves)
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