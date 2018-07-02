#ifndef MOVE_H
#define MOVE_H

#include <string>

#include "tak/util.h"

namespace Tak {

// Enum for move type
enum class MoveType {
  PlaceFlat,
  PlaceWall,
  PlaceCapstone,
  SlideLeft,
  SlideRight,
  SlideUp,
  SlideDown,
  Invalid
};

struct Move {

  // Slides are stored in locations 0xf0, 0xf00...
  // Location 0xf has the number of drops
  // Example 5a1+23 -> (3)(2)(2)
  // Example 6a1+3111 -> (1)(1)(1)(3)(4)
  Bit slide;          // if move_type = slide, then slide values
  MoveType move_type; // move type
  uint8_t pos;        // position at which move is initiated
  bool cap_move;      // whether to flatten wall at end

  Move() : move_type(MoveType::Invalid) {}
  Move(string move);

  bool operator==(const Move &rhs) const;

  int get_dpos() const;
  string to_string() const;

  inline bool is_place() const {return move_type <= MoveType::PlaceCapstone;}
  inline bool is_slide() const {return !is_place();}
  inline int slide_at(int i) const {return (slide >> (4*i + 4)) & 0xf;}
  inline int num_slide() const {return slide & 0xf;}
  inline int num_stack() const {
    int sum = 0;
    for(int i=0; i<num_slide(); ++i) sum += slide_at(i);
    return sum;
  }
};

} // namespace Tak

#endif