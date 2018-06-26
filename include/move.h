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

  // Moves are stored in locations 0xf0, 0xf00...
  // Location 0xf has the number of drops
  // Example 2,3 -> (3)(2)(2)
  // Example 3,1,1,1 -> (1)(1)(1)(3)(4)
  Bit slide;          // if move_type = slide, then slide values
  MoveType move_type; // move type
  s_int pos;          // position at which move is initiated
  bool cap_move;      // whether to flatten wall at end

  Move();
  Move(string move);
  Move(MoveType move_type, s_int pos);
  Move(MoveType move_type, s_int pos, Bit slide);

  bool operator==(const Move &rhs);

  int get_dpos();
  string to_string();

  inline bool is_place() {return move_type <= MoveType::PlaceCapstone;}
  inline bool is_slide() {return !is_place();}
  inline int slide_at(int i) {return (slide >> (4*i + 4)) & 0xf;}
  inline int num_slide() {return slide & 0xf;}
  inline int num_stack() {
    int sum = 0;
    for(int i=0; i<num_slide(); ++i) sum += slide_at(i);
    return sum;
  }
};

} // namespace Tak

#endif