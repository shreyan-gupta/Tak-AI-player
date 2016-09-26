#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <deque>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

enum Stone{
	Flat, Stand, Cap
};

namespace Types{
	typedef bool Player_Type;	// Black = false, White = true
	typedef std::pair<Stone,Player_Type> Piece;
	typedef long double eval_type;
	const bool Black = false;
	const bool White = true;
}

using namespace std;
using namespace Types;

void printVec(vector<int>&);
Piece piece(Stone, bool);

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
	bool Place_Move; 	// true if place else false
	int x;
	int y;
	Piece p; 			// rakhna hai ye/uthaana hai ye.
	char Direction; 	// + U, - D, > R, < L
	vector<int> *Drops;
	string to_string(); // print ke liye!
	Move(int, int, Piece);
	Move(int, int, char, vector<int> *);
};

struct Eval_Move{
	eval_type e;
	Move m;
	Eval_Move(eval_type&, Move&);
	Eval_Move(const Eval_Move&);
	bool operator<(const Eval_Move &other_move);
	bool operator>(const Eval_Move &other_move);
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
	void makemove(Move&);		// inputs yet to define
	void antimove(Move&);
	void generate_valid_moves(bool,std::multimap<eval_type,Move> &);
	tuple<int,int,int,int> GetStackable(int, int);
	void decide_move(Eval_Move&, bool, int, int);
};

extern vector<vector<vector<vector<int> > > > AllPerms;

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
	if(empty()) return true;
	else if(Stack.front().first == Flat) return true;
	else return false;
}

#endif