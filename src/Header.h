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
	public:
		vector< vector<Position> > GameBoard;

		Board(int);
		~Board();
		eval();
		makemove();		// inputs yet to define
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