#ifndef FEATURE_H
#define FEATURE_H

#include "tak/bitboard.h"

namespace Tak {

using eval_t = uint32_t;
using s_int = uint8_t;


// Used to get the eval of board
// Feature eval(board_size);
// eval_t eval_value = eval(board);
class Feature {
 public:
  Feature();
  eval_t operator()(const BitBoard &board);

 private:
  
};

} // namespace Tak

#endif