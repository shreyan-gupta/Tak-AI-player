#include <iostream>

#include "util.h"
#include "bitboard.h"
#include "move.h"

using namespace std;
using namespace Tak;

int main(int argc, char const *argv[])
{
  init(5);
  BitBoard state({10,1,10,1});
  cout << "Testing bitboard " << size << endl;

  Move xFa1("Fa1");
  Move xFa2("Fa2");
  Move x1a1U1("1a1+1");
  Move x2a2R11("2a2>11");

  cout << xFa1.to_string() << " " << x2a2R11.to_string() << endl;

  state.print();
  state.play_move(xFa1);    // w
  state.play_move(xFa2);    // b
  state.play_move(x1a1U1);  // w
  state.play_move(xFa1);    // b
  state.play_move(x2a2R11); // w
  state.print();
  // state.undo_move(x2a2R11);
  // state.undo_move(x1a1U1);
  // state.undo_move(xFa2);
  // state.undo_move(xFa1);
  state.print();
  return 0;
}