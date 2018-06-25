#include <iostream>

#include "bitboard.h"
#include "move.h"

using namespace std;
using namespace Tak;

size_t bit_to_int(string s){
  size_t val = 0;
  for(auto c : s){
    val <<= 1;
    val += (c == '1');
  }
  return val;
}

int main(int argc, char const *argv[])
{
  cout << "Testing bitboard" << endl;
  BitBoard::board_size = 5;
  Move::board_size = 5;
  BitBoard state;

  Move xFa1(MoveType::PlaceFlat, 0 + 0*5);
  Move xFa2(MoveType::PlaceFlat, 0 + 1*5);
  Move x1a1U1(MoveType::SlideUp, 0 + 0*5, bit_to_int("1"));
  Move x2a2R11(MoveType::SlideRight, 0 + 1*5, bit_to_int("101"));

  Move yFa1("Fa1");
  Move yFa2("Fa2");
  Move y1a1U1("1a1+1");
  Move y2a2R11("2a2>11");

  assert(xFa1 == yFa1);
  assert(x2a2R11 == y2a2R11);

  state.print();
  state.play_move(xFa1);    // w
  state.play_move(xFa2);    // b
  state.play_move(x1a1U1);  // w
  state.play_move(xFa1);    // b
  state.play_move(x2a2R11); // w
  state.print();
  state.undo_move(x2a2R11);
  // state.undo_move(x1a1U1);
  // state.undo_move(xFa2);
  // state.undo_move(xFa1);
  state.print();
  return 0;
}