#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <deque>
#include <vector>
#include <map>
#include <string>

using namespace std;

enum Player_Type{
	Black = false, White = true
};

enum Stone{
	Flat, Stand, Cap
};

namespace Types{
	typedef std::pair<Stone,Player_Type> Piece;
	typedef long double eval_type;
}

struct Position
{
	std::deque<Piece> Stack;
	int Num_Black = 0;
	int Num_White = 0;
	Piece top_piece();
	bool empty();
	string to_string();
};

struct Move
{
	bool Type; 			// false if antimove else true
	bool Place_Move; 	// true if place else false
	int x;
	int y;
	Piece p; 			// rakhna hai ye/uthaana hai ye.
	char Direction; 	// + U, - D, > R, < L
	vector<int> Drops;
	string to_string(); // print ke liye!
};

class Game
{
private:
	string to_string();
	eval_type feature1();
	eval_type feature2();
	eval_type feature3();
public:
	int size;
	vector< vector<Position> > GameBoard;
	
	eval_type eval();
	void makemove(Move);		// inputs yet to define
	void generate_valid_moves(bool,std::multimap<eval_type,Move> &);
	Game(int);
};

class Player{
private:
	int StonesLeft;
	int CapsLeft;
	bool Max;
public:
	Player(bool,int,int);
	std::pair<Move,eval_type> decide_move(Game,int);	// depth left.
};

#endif