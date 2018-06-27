#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <vector>

#include "tak/util.h"
#include "tak/move.h"

namespace Tak {

class BitBoard;

// Generates all moves for a given board. Use as below
// MoveGenerator gen(board);
// while(gen.has_next()){
//   const Move &move = gen.next();
// }

// Note that returned move changes on calling has_next()
// DO NOT call has_next() again before accessing move
// DO NOT call has_next() once it has returned false

// This implementation of MoveGenerator uses a coroutine hack

class MoveGenerator {
 public:
  MoveGenerator(BitBoard &board);
  bool has_next();
  const Move& next() {return move;}
 
 private:
  BitBoard &board;
  Move move;

  // All states required for pseudo coroutine
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