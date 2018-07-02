#ifndef FEATURE_H
#define FEATURE_H

#include "tak/util.h"
#include "tak/bitboard.h"

namespace Tak {

// Used to get the eval of board
// Feature eval(board_size);
// eval_t eval_value = eval(board);
// Note that the eval is wrt current_player
class Feature {
 public:
  eval_t operator()(const BitBoard &b);

 private:
  int delta_flat;
  int delta_wall;
  int delta_cap;

  // feature functions
  eval_t score_groups(Bit base);
  eval_t score_captive(uint32_t stack, uint8_t height, eval_t hard, eval_t soft);
  eval_t score_top_piece(const BitBoard &b);
  eval_t score_center(Bit base);
  
  // Sets the value of delta_flat, delta_wall, delta_cap
  void set_delta(const BitBoard &b);
};

} // namespace Tak

#endif