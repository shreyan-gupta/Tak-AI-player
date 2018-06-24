#ifndef MOVE_H
#define MOVE_H

#include <vector>
#include <utility>
#include <string>
#include <cstdint>

namespace Tak {

using namespace std;
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
  MoveType move_type;       // move type
  pair<s_int, s_int> pos;   // position at which move is initiated
  vector<s_int> slide;      // if move_type = slide, then slide values

  Move(MoveType &move_type, pair<s_int, s_int> &pos);
  Move(MoveType &move_type, pair<s_int, s_int> &pos, vector<s_int> &slide);
  string to_string();
};

} // namespace Tak

#endif