#include "feature.h"

namespace Tak {

Feature::Feature(s_int board_size) : board_size(board_size) {
  init();
}

void Feature::init() {
  // TODO : implement
}

eval_t operator()(const BitBoard &board) {
  // TODO : implement
  return 0;
}
 
} // namespace Tak