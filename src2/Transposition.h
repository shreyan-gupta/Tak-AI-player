#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include "Header.h"
#include "Move.h"
using namespace Types;

// u -> upper bound
// l -> lower bound
// e -> exact
// x -> uninitialized

struct Transposition{
	Move best_move;
	eval_type score;
	char flag = 'x';
	s_int depth;

	string to_string();
};

inline string Transposition::to_string()
{
	string s = "";
	s += "Move : " + best_move.to_string() + ", Score : " + to_Str(score) + ", flag = " + flag + ", Depth = " + to_Str((float)depth) + "\n";
	return s;
}

#endif