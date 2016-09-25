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

Board::Board(int size){
	this->size = size;
	GameBoard = vector< vector<Position> >(size, vector<Position>(size));
}

Board::~Board()
{
	// do nothing!
}

string Board::to_string()
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

eval_type Board::eval()
{
	// TODO
}

void Board::makemove(Move m)
{
	// TODO
	if (m.Place_move)
	{
		// x,y pe posn mein push kardo. (stack must be empty abhi.)
		if (m.Type)
		{
			// push WHAT?? 
			GameBoard[m.x][m.y].Stack.push_back(m.p);
		}
		else
		{
			// POP!
			GameBoard[m.x][m.y].Stack.pop_back();
		}
	}
	else
	{
		// move/anti?
	}
}

void Board::generate_valid_moves(bool max, std::multimap<eval_type,Move> &moves){
	// TODO
	// generating valid places
}