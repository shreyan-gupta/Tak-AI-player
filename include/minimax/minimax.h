#ifndef MINIMAX_H
#define MINIMAX_H

#include <unordered_map>

#include "tak/tak.h"
#include "minimax/transposition.h"

namespace Minimax {

using namespace Tak;

class Minimax {
 public:
  Minimax();
  void play_move(Move &move);
  Move get_move(int depth);

  // For debugging
  void print_board() {board.print();}
  void print_seq(int depth);

 private:
  BitBoard board;
  Feature eval;
  unordered_map<BitBoard, Transposition> ttable;
  
  eval_t negamax(int depth, eval_t alpha, eval_t beta);
};

} // namespace Minimax

#endif