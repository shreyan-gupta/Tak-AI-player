#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <vector>

#include "move.h"

namespace Tak {

using Bit = uint64_t;
using s_int = uint8_t;

class BitBoard;

class MoveGenerator {
 public:
  MoveGenerator(BitBoard &board);
  bool has_next();
  const Move& next() {return move;}
 
 private:
  BitBoard &board;
  Move move;

  // All states required
  int i;
  int j;
  int k;
  int STATE;
  int STATE_num_pieces;
  int STATE_num_drops;
  Bit STATE_slides;
  Bit STATE_curr_stones;
  vector<MoveType> STATE_move_type;
  vector<Bit> *STATE_slide_vec;

  Bit get_max_slide(s_int pos);
};

} // namespace Tak

#endif