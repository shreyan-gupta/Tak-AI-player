#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include <iostream>
#include "tak/util.h"
#include "tak/move.h"

#define EXACT 0x10
#define LOWERBOUND 0x20
#define UPPERBOUND 0x30
#define MAXDEPTH 0xf

namespace Minimax {

using namespace std;

// best_move stored for PV Search
// data & 0xf stores the depth
// data & 0x30 stores the flag/validity
// data & 0x40 is the lock bit
// data >> 7 stores the eval

// flag = 0x00 -> Invalid
// flag = 0x10 -> EXACT
// flag = 0x20 -> LOWERBOUND
// flag = 0x30 -> UPPERBOUND
class Transposition {

 public:
  Transposition() : data(0) {}
  bool is_valid() {return get_flag() != 0;}

  int get_depth() {return (data & 0xf);}
  const Tak::Move& get_move() {return move;}
  Tak::eval_t get_eval() {return (data >> 7);}
  int get_flag() {return (data & 0x30);}
  bool check_lock() {return (data & 0x40) != 0;}

  void set_depth(int d) {data = (data & ~0xf) | d;}
  void set_move(Tak::Move &m) {move = m;}
  void set_eval(Tak::eval_t e) {data = (data & 0x3f) | (e << 7);}
  void set_flag(int f) {data = (data & ~0x30) | f;}
  void lock() {data |= 0x40;}
  void unlock() {data &= ~0x40;}

  // For debug
  void print() {cerr << "TRANS v=" << is_valid() << " d=" << get_depth() << " f=" << get_flag() << " l=" << check_lock() << " m=" << get_move().to_string() << " e=" << get_eval() << endl;}

 private:
  Tak::Move move;
  Tak::eval_t data;
};

} // namespace Minimax

#endif