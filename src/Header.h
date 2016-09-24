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
	Player_Type getPlayer()
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
		vector<vector<Position> > GameBoard;

		Board();
		~Board();
		eval();
		makemove();		// inputs yet to define
};


#endif