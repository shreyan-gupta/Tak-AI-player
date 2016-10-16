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
};

#endif