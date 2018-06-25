#ifndef BITBOARD_H
#define BITBOARD_H

#include <vector>
#include <cstdint>

#include "move.h"

namespace Tak {

using namespace std;
using Bit = uint64_t;
using s_int = uint8_t;

class Feature;

enum class Player {
  Black,
  White
};

// represents the board class
class BitBoard {
 public:
  // board size
  static s_int board_size;
  
  BitBoard();
  bool is_valid_move(Move &move);
  void play_move(Move &move);
  void undo_move(Move &move);
  size_t hash() const;
  void print();

  friend class Feature;

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

namespace std {

template <> struct hash<Tak::BitBoard> {
  size_t operator()(const Tak::BitBoard &b) const {
    return b.hash();
  }
};

} // namespace std

#endif