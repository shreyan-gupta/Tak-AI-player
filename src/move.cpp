#include "move.h"

namespace Tak {

Move::Move(MoveType &move_type, size_t &pos) :
  move_type(move_type),
  pos(pos),
  cap_move(false),
  slide(0) {
  assert(move_type < MoveType::SlideLeft);
}

Move::Move(MoveType &move_type, size_t &pos, bool &cap_move, Bit &slide) :
  move_type(move_type),
  pos(pos),
  cap_move(cap_move),
  slide(slide) {
  assert(move_type >= MoveType::SlideLeft);
}

// Convert move to string
// Place a flat stone on the square a1 : Fa1
// Place a wall at d3 : Sd3
// Place a Capstone at b4 : Cb4
// Move 5 stones from e4, dropping two on d4 and three on c4 : 5e4<23
string Move::to_string(){
  string m = "";

  // Append coordinated
  m += (char)('a' + pos % board_size);
  m += (char)('0' + pos / board_size);

  // Append appropriate character for move type
  // TODO : Confirm the directions
  switch(move_type){
    case MoveType::PlaceFlat      : m = "F" + m; break;
    case MoveType::PlaceWall      : m = "S" + m; break; 
    case MoveType::PlaceCapstone  : m = "C" + m; break; 
    case MoveType::SlideLeft      : m = m + "<"; break; 
    case MoveType::SlideRight     : m = m + ">"; break; 
    case MoveType::SlideUp        : m = m + "+"; break; 
    case MoveType::SlideDown      : m = m + "-"; break; 
  }

  // Append sequence of slide drops and their sum
  // 0 separated bits for slide
  // Example 2,3 -> 00000110111 (binary)
  if(is_slide()){
    int temp_slide = slide;
    int sum = 0;
    string slide_seq = "";
    while(temp_slide != 0){
      int i = 0;
      assert(temp_slide & 1);
      while(temp_slide & 1){
        ++i;
        temp_slide >>= 1;
      }
      slide_seq = (char)('0' + i) + slide_seq;
      sum += i;
      temp_slide >>= 1;
    }
    m = (char)('0' + sum) + m + slide_seq;
  }
  return m;
}

} // namespace Tak