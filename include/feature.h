#ifndef FEATURE_H
#define FEATURE_H

#include "bitboard.h"

namespace Tak {

using eval_t = uint32_t;
using s_int = uint8_t;


// Used to get the eval of board
// Feature eval(board_size);
// eval_t eval_value = eval(board);
class Feature {
 public:
  Feature(s_int board_size);
  void init();
  eval_t operator()(const BitBoard &board);
 private:
  s_int board_size;
};

} // namespace Tak

#endif