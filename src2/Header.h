#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <random>
#include <cstdio>
#include <unordered_map>
#include <ctime>
#include <list>

#define RDWIN 			1000000
#define FLWIN 			1000000
#define TOPFLAT			400
#define ENDGAMEFLAT		800
#define STAND			200
#define CAP 			300
#define HARD_FCAPTIVE 	200
#define SOFT_FCAPTIVE 	-200
#define HARD_SCAPTIVE 	300
#define SOFT_SCAPTIVE 	-100
#define HARD_CCAPTIVE 	250
#define SOFT_CCAPTIVE 	-100
#define CENTER 			40
#define ENDGAMECUTOFF 	7
// #define 

namespace Types
{
	typedef int s_int;
	typedef float eval_type;
	typedef bool Player_Type;
	const bool Black = false;
	const bool White = true;

	template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str();
    }	
}

using namespace std;
using namespace Types;

void getAllPerms(s_int);

extern s_int size;
extern int TimeLimit;
extern bool opponent_type;
extern time_t start_time;
extern vector< vector< vector< vector<s_int> > > > AllPerms;

struct Player
{
	char StonesLeft;
	bool CapLeft;
	char x, y;
	Player_Type type;
	Player(bool,s_int);
	inline bool noStone();
};

inline bool Player::noStone()
{
	return (StonesLeft == 0 && !CapLeft);
}

inline string to_Str(float a)
{
	stringstream ss;
	ss << a;
	return ss.str();
}

#endif
