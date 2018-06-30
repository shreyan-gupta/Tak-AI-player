#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include <iostream>
#include "tak/move.h"

#define EXACT 0x10
#define LOWERBOUND 0x20
#define UPPERBOUND 0x30
#define MAXDEPTH 0xf

namespace Minimax {

using namespace Tak;

// best_move stored for PV Search
// data & 0xf stores the depth
// data & 0x30 stores the flag/validity
// data >> 6 stores the eval

// flag = 0x00 -> Invalid
// flag = 0x10 -> EXACT
// flag = 0x20 -> LOWERBOUND
// flag = 0x30 -> UPPERBOUND
class Transposition {

 public:
  Transposition() : data(0) {}
  bool is_valid() {return get_flag() != 0;}

  int get_depth() {return (data & 0xf);}
  const Move& get_move() {return move;}
  eval_t get_eval() {return (data >> 6);}
  int get_flag() {return (data & 0x30);}

  void set_depth(int d) {data = (data & ~0xf) | d;}
  void set_move(Move &m) {move = m;}
  void set_eval(eval_t e) {data = (data & 0x3f) | (e << 6);}
  void set_flag(int f) {data = (data & ~0x30) | f;}

  // For debug
  void print() {std::cout << "TRANS v=" << is_valid() << " d=" << get_depth() << " f=" << get_flag() << " m=" << get_move().to_string() << " e=" << get_eval() << std::endl;}

 private:
  Tak::Move move;
  eval_t data;
};

} // namespace Minimax

#endif