#ifndef MINIMAX_H
#define MINIMAX_H

#include <vector>
#include <utility>
#include <unordered_map>

#include "tak/tak.h"
#include "minimax/statistics.h"
#include "minimax/transposition.h"

namespace Minimax {

using namespace Tak;

class Minimax {
 public:
  Minimax();
  void play_move(string move) {play_move(Tak::Move(move));}
  void play_move(Move move);
  Move get_move(int depth, int max_time_ms);

  // For debugging
  void print_board() {board.print();}
  void print_seq(int depth);

 private:
  bool time_up;
  Statistics stats;

  BitBoard board;
  Feature eval;

  unordered_map<BitBoard, Transposition> ttable;
  vector<pair<Move, Move>> killer_move;

  eval_t negamax(int depth, eval_t alpha, eval_t beta);
};

} // namespace Minimax

#endif