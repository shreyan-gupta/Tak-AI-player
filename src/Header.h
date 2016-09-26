#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <deque>
#include <vector>
#include <map>
#include <string>

enum Stone{
	Flat, Stand, Cap
};

enum

namespace Types{
	typedef bool Player_Type;	// Black = false, White = true
	typedef std::pair<Stone,Player_Type> Piece;
	typedef long double eval_type;
	const bool Black = false;
	const bool While = true;
}

using namespace std;
using namespace Types;

struct Position
{
	std::deque<Piece> Stack;
	int Num_Black = 0;
	int Num_White = 0;
	Piece top_piece();
	bool empty();
	bool stackable();
	string to_string();
};

struct Move
{
	bool Type;			// false if antimove else true
	bool Place_Move; 	// true if place else false
	int x;
	int y;
	Piece p; 			// rakhna hai ye/uthaana hai ye.
	char Direction; 	// + U, - D, > R, < L
	vector<int> &Drops;
	string to_string(); // print ke liye!
	Move(int, int, Piece);
	Move(int, int, Piece, char, vector<int>);
};

struct Player{
	int StonesLeft;
	int CapsLeft;
	Player_Type type;
	Player(bool,int,int);
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
	Player p_black, p_white;

	Game(int);
	eval_type eval();
	void makemove(Move);		// inputs yet to define
	void generate_valid_moves(bool,std::multimap<eval_type,Move> &);
};

#endif