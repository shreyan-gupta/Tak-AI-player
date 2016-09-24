#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <stack>
#include <vector>

using namespace std;

typedef std::pair<Stone,Player_type> Piece;
typedef long double ll;

struct Position
{
	std::deque<Piece> Stack;
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
	bool type;
	bool place_move;
};


class Board
{
	public:
		vector< vector<Position> > GameBoard;

		Board(int);
		~Board();
		ll eval();
		void makemove(Move);		// inputs yet to define
		void generate_valid_moves(bool,std::multimap<ll,Move> &);
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
	bool Max;
public:
	Player(bool,int,int);
	~Player();
	std::pair<Move,ll> decide_move(Board,int);
};

#endif