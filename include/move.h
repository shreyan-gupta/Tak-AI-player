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
  static s_int board_size;  // board size

  MoveType move_type; // move type
  s_int pos;         // position at which move is initiated
  bool cap_move;      // whether to flatten wall at end
  Bit slide;          // if move_type = slide, then slide values
  // 0 separated bits for slide
  // Example 2,3 -> 00000110111 (binary)

  Move();
  Move(string move);
  Move(MoveType move_type, s_int pos);
  Move(MoveType move_type, s_int pos, Bit slide);

  bool operator==(const Move &rhs);

  int get_dpos();
  bool is_place() {return move_type <= MoveType::PlaceCapstone;}
  bool is_slide() {return !is_place();}
  string to_string();
};

} // namespace Tak

#endif