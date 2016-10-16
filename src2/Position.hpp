#ifndef POSITION_H
#define POSITION_H

#include "Header.h"

// F S C -> White
// f s c -> Black
// A < 95

struct Position{
	string stack;
	s_int num_black = 0;
	s_int num_white = 0;
	bool empty();
	bool stackable();
	bool capable();
	void top5(pair<s_int, s_int> &);
	string to_string();
};

inline bool Position::empty(){
	return stack.empty();
}

inline bool Position::stackable(){
	if(stack.empty()) return true;
	else if(stack.back() == 'F' || stack.back() == 'f') return true;
	else return false;
}

inline bool Position::capable(){
	if(stack.empty()) return false;
	else if(stack.back() == 'S' || stack.back() == 's') return true;
	else return false;
}

inline void Position::top5(pair<s_int, s_int> &p){
	s_int count = 0;
	p.first = p.second = 0;
	for(auto it = stack.rbegin(); it != stack.rend() && count < 5; ++it, ++count){
		if(*it < 95) ++p.first;
		else ++p.second;
	}
}

inline string to_string(){
	return stack;
}

#endif