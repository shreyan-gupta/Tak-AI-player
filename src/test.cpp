#include <iostream>

#include "util.h"
#include "bitboard.h"
#include "move.h"
#include "move_generator.h"

using namespace std;
using namespace Tak;

int main(int argc, char const *argv[])
{
  init(5);
  BitBoard state;
  cout << "Testing bitboard " << size << endl;

  string str;
  while(true){
    cin >> str;
    if(str == "end") break;
    if(str[0] == '#') continue;
    if(str == "move_gen"){
      cout << str << endl;
      MoveGenerator mg(state);
      while(mg.has_next()){
        cout << mg.next().to_string() << endl;
      }
      continue;
    }
    Move move(str);
    state.play_move(move);
    state.print();
  }
  return 0;
}