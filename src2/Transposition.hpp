#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include "Header.h"
#include "Move.hpp"
using namespace Types;

// u -> upper bound
// l -> lower bound
// e -> exact

struct Transposition{
	Move best_move;
	eval_type score;
	char flag;
	s_int depth;
};

#endif