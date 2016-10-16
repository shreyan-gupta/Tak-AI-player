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

namespace Types
{
	typedef char s_int;
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

void getAllPerms(char);

extern s_int size;
extern vector< vector< vector< vector<s_int> > > > AllPerms;

struct Player
{
	char StonesLeft;
	bool CapLeft;
	char x, y;
	Player_Type type;
	Player(bool,char);
};

#endif