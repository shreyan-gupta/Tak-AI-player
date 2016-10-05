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
#include <unordered_map>
#include <ctime>


// w[0] = 100000 -> Path 
// w[1] = Closer to center -> abs(i-half) + abs(j-half)

// w[4] -> your pieces in top 5 of any your stack.
// w[5] -> oppo ke pieces below yours

// w[6] -> Flat, Stack top
// w[7] -> Stand, Stack top
// w[8] -> Cap, Stack top

// w[9]  -> Flat 	Flat 	same
// w[10] -> Flat 	Stand 	same
// w[11] -> Flat 	Cap 	same
// w[12] -> Stand 	Stand 	same
// w[13] -> Stand 	Cap 	same
// w[14] -> Flat 	Stand 	diff
// w[2]  -> Flat 	Cap 	diff
// w[3]  -> Stand 	Cap 	diff

// w[15] -> No piece left win

// w[16] -> sum of influence

enum Stone{
	Flat, Stand, Cap
};

class Game;

namespace Types{
	typedef bool Player_Type;	// Black = false, White = true
	typedef std::pair<Stone,Player_Type> Piece;
	typedef float eval_type;
	typedef eval_type (Game::*Feature) (void);
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
	bool checkfavourable();
	void print_index();
}

using namespace std;
using namespace Types;


struct Position
{
	std::deque<Piece> Stack;
	char Num_Black = 0;
	char Num_White = 0;
	Piece top_piece();
	bool empty();
	bool stackable();
	bool capable();
	string to_string();
	void top5(pair<char,char> &);
};

struct Move
{
	bool CapMove = false;
	bool Place_Move; 	// true if place else false
	char x;
	char y;
	Piece p; 			// rakhna hai ye/uthaana hai ye.
	char Direction; 	// + U, - D, > R, < L
	vector<char> *Drops;
	string to_string(); // print ke liye!
	Move(char, char, Piece);
	Move(char, char, char, vector<char> *);
	// Move()
	Move();
	// Move(string);
};

struct Eval_Move{
	eval_type e;
	Move m;
	Eval_Move();
	Eval_Move(eval_type&, Move&);
	Eval_Move(const Eval_Move&);
	bool operator<(const Eval_Move &other_move);
	bool operator>(const Eval_Move &other_move);
};

struct Player{
	char StonesLeft;
	bool CapLeft;
	char x, y;
	Player_Type type;
	Player(bool,char);
};


// void printVec(vector<int>&);
Piece piece(Stone, bool);
void getAllPerms(char);
// int favourableStack(vector< vector< Position> > &, int, int);


class Game
{
private:
	unordered_map<string, eval_type> duplicates;
	eval_type feature0();
	eval_type feature1();
	// eval_type feature2();
	// eval_type feature3();
	// eval_type feature4();
	// eval_type feature5();
	eval_type nbr(Piece, Piece);
	eval_type center(char, char);
	eval_type neighbor(char, char);
	eval_type stone_weight(Stone);
	char sq(char, char);
	eval_type top_colors(char, char, std::pair<char,char> &);
public:
	string to_string();
	char size;
	vector< vector<Position> > GameBoard;
	Feature f[2];
	eval_type w[18];
	Player p_black, p_white;

	Game(char, char);
	eval_type eval();
	void makemove(Move&);		// inputs yet to define
	void antimove(Move&);
	void generate_valid_moves(bool,std::multimap<eval_type,Move> &);
	void GetStackable(char, char, bool, vector<char>&);
	void decide_move(Eval_Move&, bool, char, char, eval_type, eval_type);
	void UpdatePlayer(Player_Type, Move&, bool);
	void make_opponent_move(string,bool);
};

extern vector<vector<vector<vector<char> > > > AllPerms;
extern char Size;
extern bool opponent_type;
extern int TimeLimit;
extern time_t start_time;

extern int prune_count[10];
extern int prune_index[10];

extern int depth_count[10];
extern time_t total_time;

// extern int sum_index[10];
// extern int count_index[10];
// extern int max_index[10];

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
		// cerr << "stand! \n";
		return true;
	}
	else return false;
}

inline void Position::top5(pair<char,char> &p)
{
	char count = 0;
	p.first = 0;
	p.second = 0;
	for (auto it = Stack.begin() ; it != Stack.end() && count < 5 ; it ++)
	{
		if ((*it).second == White) p.first ++;
		else p.second ++;
		count ++;
	}
}

#endif