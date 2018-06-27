#ifndef BITBOARD_H
#define BITBOARD_H

#include <vector>
#include <cstdint>

#include "tak/util.h"
#include "tak/move.h"

namespace Tak {

// Forward declare class Feature
class Feature;
class MoveGenerator;

// Represents the board class
// Deals with checking, playing and undoing moves
// Note that the init() function should be called before
// declaring a BitBoard instance
class BitBoard {
 public:  
  BitBoard(Pieces pieces = default_pieces);
  bool is_valid_move(Move &move) const;
  void play_move(const Move &move);
  void undo_move(const Move &move);
  size_t hash() const;
  void print() const;

  bool operator==(const BitBoard &rhs) const;

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
  // Top of the stack is least significant bit
  std::vector<s_int> height;
  std::vector<s_int> stack;

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