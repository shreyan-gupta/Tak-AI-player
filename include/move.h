#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <cstdint>

namespace Tak {

using namespace std;
using Bit = uint64_t;
using s_int = uint8_t;

// Enum for move type
enum class MoveType {
  PlaceFlat,
  PlaceWall,
  PlaceCapstone,
  SlideLeft,
  SlideRight,
  SlideUp,
  SlideDown
};

struct Move {
  static int board_size;  // board size

  MoveType move_type; // move type
  s_int pos;         // position at which move is initiated
  bool cap_move;      // whether to flatten wall at end
  Bit slide;          // if move_type = slide, then slide values
  // 0 separated bits for slide
  // Example 2,3 -> 00000110111 (binary)

  Move(MoveType &move_type, size_t &pos);
  Move(MoveType &move_type, size_t &pos, bool &cap_move, Bit &slide);
  string to_string();
  bool is_place() {return move_type <= MoveType::PlaceCapstone;}
  bool is_slide() {return !is_place();}
};

} // namespace Tak

#endif