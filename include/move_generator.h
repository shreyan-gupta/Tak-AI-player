#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "bitboard.h"

namespace Tak {

class MoveGenerator {
 public:
  MoveGenerator(BitBoard &board);
  bool has_next() {return done;}
  Move& next();
 private:
  // Check if it's valid to hold reference
  BitBoard &board;
  Move move;
  bool done;
};

} // namespace Tak

#endif