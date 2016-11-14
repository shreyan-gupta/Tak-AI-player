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
#include <limits.h>
#include <assert.h>

// #define 

namespace Types
{
	typedef int s_int;
	typedef int eval_type;
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

extern const eval_type FLAT_MUL		 ;
extern const eval_type RDWIN 		 ;
extern const eval_type FLWIN 		 ;
extern const eval_type ENDGAMEFLAT	 ;
extern const eval_type TOPFLAT		 ;
extern const eval_type STAND		 ;
extern const eval_type CAP 			 ;
extern const eval_type HARD_FCAPTIVE ;
extern const eval_type SOFT_FCAPTIVE ;
extern const eval_type HARD_SCAPTIVE ;
extern const eval_type SOFT_SCAPTIVE ;
extern const eval_type HARD_CCAPTIVE ;
extern const eval_type SOFT_CCAPTIVE ;
extern const eval_type CENTER 		 ;
extern const eval_type ENDGAMECUTOFF ; 
extern vector<eval_type> GROUP;

void getAllPerms(s_int);

extern s_int size,pieces;
extern int TimeLimit;
extern bool opponent_type;
extern time_t start_time;
extern vector< vector< vector< vector<s_int> > > > AllPerms;
extern vector<int> GroupWt;

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
