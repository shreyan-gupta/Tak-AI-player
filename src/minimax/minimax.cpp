#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

#include "minimax/minimax.h"
#include "minimax/transposition.h"

using namespace std;
using namespace std::chrono;

namespace Minimax {

Minimax::Minimax() :
  time_up(false),
  killer_move(vector<pair<Move, Move>>(0xf)) {}

void Minimax::play_move(Move move){
  if(board.is_valid_move(move)) board.play_move(move);
  else assert(false);
}

// currently with iterative deepning
Move Minimax::get_move(int depth, int max_time_ms){

  // Keep yielding till timeout or ids is finished
  auto timer = [&](){
    auto end_time = steady_clock::now() + milliseconds(max_time_ms);
    while(!time_up && steady_clock::now() < end_time){
      this_thread::yield();
    }
    time_up = true;
  };

  time_up = false;
  thread timer_thread(timer);
  stats.clear();
  for(int d=1; d<=depth; ++d){
    eval_t score = negamax(d, -Weights::INF, Weights::INF);
    print_seq(d);
  }
  time_up = true;
  timer_thread.join();
  stats.print(depth);
  cerr << "Transposition Table size: " << ttable.size() << endl;
  return ttable[board].get_move();
}

// For debugging
void Minimax::print_seq(int depth){
  BitBoard temp_board = board;
  cerr << "ids d=" << depth << " score=" << ttable[temp_board].get_eval() << "\t";
  for(int i=0; i<depth; ++i){
    auto &t = ttable[temp_board];
    if(t.get_move().move_type == MoveType::Invalid) break;
    cerr << t.get_move().to_string() << " ";
    temp_board.play_move(t.get_move());
  }
  cerr << endl;
}

// negamax algorithm
eval_t Minimax::negamax(int depth, eval_t alpha, eval_t beta) {
  if(time_up) return Weights::INF;

  // fetch values from transposition table
  stats.num_minimax_call[depth]++;
  auto alpha_orig = alpha;
  auto &t = ttable[board];
  if(!t.is_valid()){
    // Assume that eval returns value wrt current_player
    // Ensures eval(board) called only once
    eval_t curr_eval = eval(board);
    t.set_eval(curr_eval);
    t.set_flag(EXACT);
    // If either player has won, make terminal board
    // On returning to this state with any depth, we return eval
    if(std::abs(curr_eval) > Weights::CHECKWIN){
      t.set_depth(MAXDEPTH);
    }
  }
  
  // Base case, depth = 0 -> t.flag = EXACT and t.depth >= 0
  // Base case, abs(eval) = WIN -> 
  if(t.get_depth() >= depth){
    switch(t.get_flag()){
      case EXACT      : return t.get_eval();
      case LOWERBOUND : alpha = std::max(alpha, t.get_eval());
      case UPPERBOUND : beta = std::min(beta, t.get_eval());
    }
    if(alpha >= beta) return t.get_eval();
  }
  else negamax(depth-1, alpha, beta);
  // In case t.depth < depth, then we call ids with depth-1
  // This ensures the ttable is filled with t.depth = depth-1

  bool done = false;
  Move best_move;
  eval_t best_val = -Weights::INF;

  // Try move m
  auto try_move = [&](const Move &m) {
    // Play the move m
    board.play_move(m);
    eval_t child = -negamax(depth-1, -beta, -alpha);
    board.undo_move(m);

    alpha = std::max(alpha, child);
    if(child > best_val){
      best_move = m;
      best_val = child;
    }
    if(alpha >= beta || best_val > Weights::CHECKWIN) done = true;
  };

  // Update transposition table
  auto update_transposition = [&]() {
    t.set_move(best_move);
    t.set_eval(best_val);
    // If win, always take best_move
    if(best_val > Weights::CHECKWIN) {
      t.set_depth(MAXDEPTH);
      t.set_flag(EXACT);
    }
    else {
      t.set_depth(depth);
      if(best_val <= alpha_orig) t.set_flag(UPPERBOUND);
      else if(best_val >= beta) t.set_flag(LOWERBOUND);
      else t.set_flag(EXACT);
    }
  };

  // PV Move
  if(!time_up && t.get_depth() > 0) {
    const Move &move = t.get_move();
    try_move(move);
    if(done) stats.prune_pv[depth]++;
  }

  // Killer move 1
  if(!time_up && !done){  
    Move move = killer_move[depth].first;
    if(board.is_valid_move(move)) try_move(move);
    if(done) stats.prune_killer1[depth]++;
  }

  // Killer move 2
  if(!time_up && !done){
    Move move = killer_move[depth].second;
    if(board.is_valid_move(move)) try_move(move);
    if(done) std::swap(killer_move[depth].first, killer_move[depth].second);
    if(done) stats.prune_killer2[depth]++;
  }

  // Iterate through all other moves
  if(!time_up && !done){
    MoveGenerator gen(board);
    while(gen.has_next()){
      const Move &move = gen.next();
      try_move(move);
      if(time_up || done) break;
    }
    // Append best_move to killer_move list
    if(!time_up){
      killer_move[depth].second = killer_move[depth].first;
      killer_move[depth].first = best_move;
    }
    stats.prune_other[depth]++;
  }

  if(!time_up) update_transposition();
  return best_val;
}

} // namespace Minimax