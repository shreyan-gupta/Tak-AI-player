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
}

void test_minimax(){
  Minimax::Minimax m;
  
  string str;
  while(true){
    cin >> str;
    if(str == "end") break;
    if(str[0] == '#') continue;
    Move move(str);
    cout << "my_move " << move.to_string() << endl;
    m.play_move(move);
    auto start = high_resolution_clock::now();
    Move minimax_move = m.get_move(5);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "minimax_move " << minimax_move.to_string() << endl;
    cout << "duration " << duration.count() << endl;
    m.play_move(minimax_move);
    m.print_board();
  }
}

int main(int argc, char const *argv[])
{
  Tak::init(5);
  test_minimax();
  return 0;
}