#define DEBUG

#include <iostream>
#include <algorithm>

#include "minimax/minimax.h"
#include "minimax/transposition.h"

using namespace std;

namespace Minimax {

Minimax::Minimax() {}

void Minimax::play_move(Move &move){
  if(board.is_valid_move(move)) board.play_move(move);
  else assert(false);
}

// currently with iterative deepning
Move Minimax::get_move(int depth){
  for(int d=1; d<=depth; ++d){
    eval_t score = negamax(d, -Weights::INF, Weights::INF);
    #ifdef DEBUG
      cout << "ids d=" << d << " score=" << score << " \t";
      print_seq(d);
    #endif
  }
  return ttable[board].get_move();
}

// For debugging
void Minimax::print_seq(int depth){
  BitBoard temp_board = board; 
  for(int i=0; i<depth; ++i){
    auto &t = ttable[temp_board];
    cout << t.get_move().to_string() << " ";
    temp_board.play_move(t.get_move());
  }
  cout << endl;
}

// negamax algorithm
eval_t Minimax::negamax(int depth, eval_t alpha, eval_t beta) {
  
  // fetch values from transposition table
  auto alpha_orig = alpha;
  auto &t = ttable[board];
  // Ensures eval(board) called only once
  if(!t.is_valid()){
    // Assume that eval returns value wrt current_player
    eval_t curr_eval = eval(board);
    t.set_eval(curr_eval);
    t.set_flag(EXACT);
    // If either player has won, make terminal board
    // On returning to this state with any depth, we return eval
    if(std::abs(curr_eval) > Weights::CHECKWIN){
      t.set_depth(MAXDEPTH);
    }
  }
  // Base case, depth = 0 or some player has won, t.depth = 15 
  if(t.get_depth() >= depth){
    switch(t.get_flag()){
      case EXACT      : return t.get_eval();
      case LOWERBOUND : alpha = std::max(alpha, t.get_eval());
      case UPPERBOUND : beta = std::min(beta, t.get_eval());
    }
    if(alpha >= beta) return t.get_eval();
  }

  // Iterate through all the moves
  Move best_move;
  eval_t best_val = -Weights::INF;
  MoveGenerator gen(board);
  while(gen.has_next()){
    const Move &move = gen.next();
    
    #ifdef DEBUG
      // TESTING PLAY_MOVE, UNDO_MOVE
      if(!board.is_valid()){
        board.print();
        cout << "STATE ERROR!!!!!" << endl;
      }
      Move temp_move = move;
      bool valid_move = board.is_valid_move(temp_move);
      if(!valid_move || (temp_move.cap_move != move.cap_move)){
        board.print();
        cout << "MOVE ERROR!!!! MOVE " << move.to_string() << " CAP " << move.cap_move << temp_move.cap_move << endl;
        assert(false);
      }
      auto temp_board = board;
      board.play_move(move);
      board.undo_move(move);
      if(board != temp_board){
        temp_board.print();
        board.print();
        cout << "UNDO ERROR!!!!! MOVE " << move.to_string() << " CAP " << move.cap_move << endl;
        assert(false);
      }
      // END TESTING
    #endif

    board.play_move(move);
    eval_t child = -negamax(depth-1, -beta, -alpha);
    board.undo_move(move);
    
    #ifdef DEBUG
      if(depth == 5) cout << "depth " << depth << " " << move.to_string() << " " << child << endl;
      // if(depth == 2) cout << "depth " << depth << " " << move.to_string() << " " << child << endl;
    #endif

    alpha = std::max(alpha, child);
    if(child > best_val){
      best_move = move;
      best_val = child;
    }
    // Set max_depth in transposition table
    if(child > Weights::CHECKWIN){
      depth = MAXDEPTH;
      break;
    }
    if(alpha >= beta) break;
  }

  // Update transposition table
  t.set_depth(depth);
  t.set_move(best_move);
  t.set_eval(best_val);
  if(best_val <= alpha_orig) t.set_flag(UPPERBOUND);
  else if(best_val >= beta) t.set_flag(LOWERBOUND);
  else t.set_flag(EXACT);

  return best_val;
}

} // namespace Minimax