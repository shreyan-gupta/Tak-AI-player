#ifndef BITBOARD_H
#define BITBOARD_H

#include <vector>
#include <cstdint>

#include "move.h"

namespace Tak {

using namespace std;
using Bit = uint64_t;
using s_int = uint8_t;

enum class Player {
  Black,
  White
};

// represents the board class
class BitBoard {
 public:
  // board size
  static int board_size;
  
  BitBoard();
  void play_move(Move &move);
  void undo_move(Move &move);

 private:
  Player current_player;

  Bit black_stones; // all black owned positions
  Bit white_stones; // all white owned positions
  Bit wall_stones;  // position of all walls
  Bit cap_stones;   // position of all capstones

  // each position of board
  vector<s_int> height;
  vector<s_int> pieces;

  // TODO : need to define a hash
};

} // namespace Tak

#endif