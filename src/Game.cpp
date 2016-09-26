#include "Header.h"
using namespace Types;

Player::Player(bool player_type, int pieces, int caps){
	type = player_type;
	StonesLeft = pieces;
	CapsLeft = caps;
}

Move::Move(int x, int x, Piece p){
	this->Type = true;
	this->Place_Move = true;
	this->x = x;
	this->y = y;
	this->p = p;
	this->Direction;
	this->Drops;
}

Move::Move(int x, int y, Piece p, char d, vector<int> v){
	this->Type = true;
	this->Place_Move = false;
	this->x = x;
	this->y = y;
	this->p = p;
	this->Direction = d;
	this->Drops = v;
}

inline Piece piece(Stone s, bool p){
	return make_pair(s,p);
}

inline Piece Position::top_piece(){
	return Stack.front();
}

inline bool Position::empty(){
	return Stack.empty();
}

inline bool Position::stackable(){
	if(empty) return true;
	else if(Stack.front().first == Flat) return true;
	else return false;
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
	p_white = Player(White, 100, 1);
	p_black = Player(Black, 100, 1);
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
			int pick_x = m.x + x_add*d.size();
			int pick_y = m.y + y_add*d.size();
		}
	}
}

void Game::generate_valid_moves(bool player, multimap<eval_type,Move> &move){
	// FIX P_BLACK
	if(player == Black && p_black.CapsLeft != 0 || player == White && p_white.CapsLeft != 0)
	// if(p_black.CapsLeft != 0)
	for(int i=0; i<size; ++i){
		for(int j=0; j<size; ++j){
			if(!GameBoard[i][j].empty()){
				Move m(i, j, piece(Cap,player));
				makemove(m);
				moves.insert(eval(), m);
				m.Type = false;
				makemove(m);
				m.Type = true;
			}
		}
	}
	if(player==Black && p_black.StonesLeft != 0 || player==White && p_white.StonesLeft != 0)
	for(int i=0; i<size; ++i){
		for(int j=0; j<size; ++j){
			if(!GameBoard[i][j].empty()){
				Move m1(i, j, piece(Flat,player));
				Move m2(i, j, piece(Stand,player));
				moves.insert(eval(???), m1);
				moves.insert(eval(???), m2);
			}
		}
	}
}


