#include <iostream>

#include "tak/util.h"
#include "tak/bitboard.h"
#include "tak/move.h"
#include "tak/move_generator.h"
#include "tak/feature.h"

using namespace std;
using namespace Tak;

int main(int argc, char const *argv[])
{
  init(5);
  BitBoard state;
  Feature eval;
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
    cout << "EVAL : " << eval(state) << endl;
  }
  return 0;
}