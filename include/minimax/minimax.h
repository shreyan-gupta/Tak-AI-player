#ifndef MINIMAX_H
#define MINIMAX_H

#include "tak/tak.h"

namespace Minimax {

using namespace Tak;

class Minimax {
 public:
  Minimax();
  void play_move(Move &move);
  Move get_move(int depth);

  void print_board() {board.print();}

 private:
  BitBoard board;
  Feature eval;

  Move return_move;
  
  eval_t negamax(int depth, eval_t alpha, eval_t beta);
};

} // namespace Minimax

#endif