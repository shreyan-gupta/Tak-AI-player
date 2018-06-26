#ifndef BITBOARD_H
#define BITBOARD_H

#include <vector>
#include <cstdint>

#include "util.h"
#include "move.h"

namespace Tak {

using namespace std;
using Bit = uint64_t;
using s_int = uint8_t;

// Forward declare class Feature
class Feature;
class MoveGenerator;

// represents the board class
class BitBoard {
 public:  
  BitBoard(Pieces pieces = default_pieces);
  bool is_valid_move(Move &move);
  void play_move(Move &move);
  void undo_move(Move &move);
  size_t hash() const;
  void print();

  bool operator==(const BitBoard &rhs);

  friend class Feature;
  friend class MoveGenerator;

 private:
  Player current_player;
  Pieces pieces;

  Bit black_stones; // all black owned positions
  Bit white_stones; // all white owned positions
  Bit wall_stones;  // position of all walls
  Bit cap_stones;   // position of all capstones

  // each position of board
  vector<s_int> height;
  vector<s_int> stack;

  void set_player_at_pos(s_int pos);
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