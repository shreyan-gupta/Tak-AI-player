#ifndef POSITION_H
#define POSITION_H

#include "Header.h"

// F S C -> White
// f s c -> Black

struct Position{
	string stack;
	s_int num_black = 0;
	s_int num_white = 0;
	bool empty();
	bool stackable();
	bool capable();
};

inline bool Position::empty(){
	return stack.empty();
}

#endif