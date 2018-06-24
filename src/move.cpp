#include "move.h"

namespace Tak {

Move::Move(MoveType &move_type, pair<s_int, s_int> &pos) :
  move_type(move_type),
  pos(pos) {
  assert(move_type < MoveType::SlideLeft);
}

Move::Move(MoveType &move_type, pair<s_int, s_int> &pos, vector<s_int> &slide) :
  move_type(move_type),
  pos(pos),
  slide(slide) {
  assert(move_type >= MoveType::SlideLeft);
}

// Convert move to string
// Place a flat stone on the square a1 : Fa1
// Place a wall at d3 : Sd3
// Place a capstone at b4 : Cb4
// Move 5 stones from e4, dropping two on d4 and three on c4 : 5e4<23
string Move::to_string(){
  string m = "";

  // Append coordinated
  m += (char)('a' + pos.first);
  m += (char)('0' + pos.second);

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
  if(move_type >= MoveType::SlideLeft){
    int sum = 0;
    for(auto &i : slide){
      m += (char)('0' + i);
      sum += i;
    }
    m = (char)('0' + sum) + m;
  }
  return m;
}

} // namespace Tak