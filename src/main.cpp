#include <iostream>
#include <algorithm>
#include <chrono>

#include "tak/tak.h"
#include "minimax/minimax.h"

using namespace std;
using namespace std::chrono;

milliseconds time_left;

int get_time_ms(){
  auto t = time_left.count();
  return std::min(10000ll, t/10);
}

int main(int argc, char const *argv[])
{
  // Take inputs
  int player_no, size, time_limit;
  cerr << "Enter player_no, size, time_limit(seconds)" << endl;
  cin >> player_no >> size >> time_limit;

  // Decrease time limit by 0.5 second for safety
  time_left = milliseconds(time_limit*1000 - 500);

  Tak::init(size);
  Minimax::Minimax game;

  if(player_no == 1){
    // If player 1, place opponent's piece first
    string white_move, black_move = "Fa1";
    cout << black_move << endl;
    cin >> white_move;

    game.play_move(white_move);
    game.play_move(black_move);
  }
  else{
    // If player 2, receive location of your piece
    string white_move = "Fa1", black_move;
    cin >> black_move;
    if(black_move == "Fa1"){
      white_move = "Fa";
      white_move += (char)('0' + size);
    }
    cout << white_move << endl;

    game.play_move(white_move);
    game.play_move(black_move);

    // Get second move of player 1
    cin >> white_move;
    game.play_move(white_move);
  }

  while(true){
    string opponent_move;
    auto start_time = steady_clock::now();
    Tak::Move my_move = game.get_move(7, get_time_ms());
    game.play_move(my_move);
    cout << my_move.to_string() << endl;
    auto time_spent = duration_cast<milliseconds>(steady_clock::now() - start_time);
    time_left -= time_spent;
    game.print_board();
    cerr << "Time left " << time_left.count() << " Time spent " << time_spent.count() << endl;
    cin >> opponent_move;
    game.play_move(opponent_move);
  }

  return 0;
}