#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <stack>
#include <vector>

using namespace std;

typedef std::pair<Stone,Player_type> Piece;

struct Position
{
	std::stack<Piece> Stack;
	int black = 0;
	int white = 0;
	string to_string();
	inline Player_Type get_player()
	{
		if (Stack.size() == 0)
			return none;
		else
			return Stack.top().second;
	}
};

struct Move
{
	
};


class Board
{
		string to_string();
	public:
		vector< vector<Position> > GameBoard;
		Board(int);
		~Board();
		eval();
		makemove();	// inputs yet to define
};


enum Player_type{
	Black, White, None
};

enum Stone{
	F,S,C
};

class Player{
private:
	int StonesLeft;
	int CapsLeft;
	bool MinMax;
public:
	Player(bool,int,int);
	~Player();
	decide_move();
};

#endif