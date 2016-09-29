#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <deque>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <random>
#include <cstdio>

enum Stone{
	Flat, Stand, Cap
};

class Game;

namespace Types{
	typedef bool Player_Type;	// Black = false, White = true
	typedef std::pair<Stone,Player_Type> Piece;
	typedef float eval_type;
	typedef int (Game::*Feature) (void);
	const bool Black = false;
	const bool White = true;

	template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str();
    }
}

namespace Test
{
	bool checkMove();
	bool checkValid();
	bool checkPath();
}

using namespace std;
using namespace Types;

void printVec(vector<int>&);
Piece piece(Stone, bool);
void getAllPerms(int);

struct Position
{
	std::deque<Piece> Stack;
	int Num_Black = 0;
	int Num_White = 0;
	Piece top_piece();
	bool empty();
	bool stackable();
	bool capable();
	string to_string();
};

struct Move
{
	bool CapMove = false;
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
	bool CapsLeft;
	int x, y;
	Player_Type type;
	Player(bool,int);
};

class Game
{
private:
	int feature0();
	int feature1();
	int feature2();
	int feature3();
public:
	string to_string();
	int size;
	vector< vector<Position> > GameBoard;
	Feature f[4];
	eval_type weight[4];
	Player p_black, p_white;

	Game(int);
	eval_type eval();
	void makemove(Move&);		// inputs yet to define
	void antimove(Move&);
	void generate_valid_moves(bool,std::multimap<eval_type,Move> &);
	tuple<int,int,int,int> GetStackable(int, int, bool);
	void decide_move(Eval_Move&, bool, int, int);
	void UpdatePlayer(Player_Type, Move&, bool);
};

extern vector<vector<vector<vector<int> > > > AllPerms;
extern int Size;

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

inline bool Position::capable()
{
	if (empty()) return false;
	else if (Stack.front().first == Stand)
	{
		// cout << "stand! \n";
		return true;
	}
	else return false;
}
#endif