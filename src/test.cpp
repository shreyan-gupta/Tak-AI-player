#include <iostream>
#include <chrono>

#include "tak/tak.h"
#include "minimax/minimax.h"

using namespace std;
using namespace std::chrono;
using namespace Tak;
// using namespace Minimax;

void test_board(){
  BitBoard state;
  Feature eval;
  cerr << "Testing bitboard " << size << endl;

  string str;
  while(true){
    cin >> str;
    if(str == "end") break;
    if(str[0] == '#') continue;
    if(str == "move_gen"){
      cerr << str << endl;
      MoveGenerator mg(state);
      while(mg.has_next()){
        const auto &move = mg.next();
        cerr << move.to_string() << " cap_move " << move.cap_move << endl;
      }
      continue;
    }
    Move move(str);
    state.play_move(move);
    state.print();
    cerr << "EVAL : " << eval(state) << endl;
  }
}

void test_minimax(){
  Minimax::Minimax m;
  
  string str;
  while(true){
    cin >> str;
    if(str == "end") break;
    if(str[0] == '#') continue;
    Move move(str);
    cerr << "my_move " << move.to_string() << endl;
    m.play_move(move);
    auto start = high_resolution_clock::now();
    Move minimax_move = m.get_move(5, 10000);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cerr << "minimax_move " << minimax_move.to_string() << endl;
    cerr << "duration " << duration.count() << endl;
    m.play_move(minimax_move);
    m.print_board();
  }
}

int main(int argc, char const *argv[])
{
  Tak::init(7);
  cerr << "size " << size << endl;
  test_minimax();
  return 0;
}