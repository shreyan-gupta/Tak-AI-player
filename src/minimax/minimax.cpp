#include <algorithm>
#include <iostream>

#include "minimax/minimax.h"

using namespace std;

namespace Minimax {

Minimax::Minimax() {}

void Minimax::play_move(Move &move){
  if(board.is_valid_move(move)) board.play_move(move);
  else assert(false);
}

Move Minimax::get_move(int depth){
  eval_t score = negamax(depth, -Weights::INF, Weights::INF);
  return return_move;
}

eval_t Minimax::negamax(int depth, eval_t alpha, eval_t beta) {
  // Base case, depth = 0
  // Base case, either player has won
  // Assume that eval returns value wrt current_player
  eval_t curr_eval = eval(board);
  if(depth == 0) return curr_eval;
  if(std::abs(curr_eval) > Weights::CHECKWIN) return curr_eval;

  Move best_move;
  eval_t best_val = -Weights::INF;
  MoveGenerator gen(board);
  while(gen.has_next()){
    const Move &move = gen.next();
    
    board.play_move(move);
    eval_t child = -negamax(depth-1, -beta, -alpha);
    board.undo_move(move);

    alpha = std::max(alpha, child);
    if(child > best_val){
      best_move = move;
      best_val = child;
    }
    if(alpha >= beta) break;
    if(child > Weights::CHECKWIN) break;
  }
  return_move = best_move;
  return best_val;
}

} // namespace Minimax