#include "tak/util.h"
#include "tak/move.h"

namespace Tak {

Move::Move() :
  move_type(MoveType::PlaceFlat),
  pos(0),
  cap_move(false),
  slide(0) {}

Move::Move(string move){  
  pos = (move[1] - 'a') + size*(move[2] - '1');


  if(move[0] == 'F') move_type = MoveType::PlaceFlat;
  else if(move[0] == 'S') move_type = MoveType::PlaceWall;
  else if(move[0] == 'C') move_type = MoveType::PlaceCapstone;
  else if(move[3] == '<') move_type = MoveType::SlideLeft;
  else if(move[3] == '>') move_type = MoveType::SlideRight;
  else if(move[3] == '+') move_type = MoveType::SlideUp;
  else if(move[3] == '-') move_type = MoveType::SlideDown;

  cap_move = false;
  slide = 0;

  if(is_slide()){
    for(int i=move.size()-1; i>=4; --i){
      slide |= (move[i] - '0');
      slide <<= 4;
    }
    slide |= (move.size() - 4);
  }
}

Move::Move(MoveType move_type, s_int pos) :
  move_type(move_type),
  pos(pos),
  cap_move(false),
  slide(0) {
  assert(move_type < MoveType::SlideLeft);
}

Move::Move(MoveType move_type, s_int pos, Bit slide) :
  move_type(move_type),
  pos(pos),
  cap_move(false),
  slide(slide) {
  assert(move_type >= MoveType::SlideLeft);
}

bool Move::operator==(const Move &rhs) const {
  return(
    move_type == rhs.move_type &&
    pos == rhs.pos &&
    slide == rhs.slide
  );
}

int Move::get_dpos() const {
  switch(move_type){
    case MoveType::SlideLeft  : return -1;
    case MoveType::SlideRight : return 1;
    case MoveType::SlideUp    : return size;
    case MoveType::SlideDown  : return -size;
    default : assert(false);
  }
}

// Convert move to string
// Place a flat stone on the square a1 : Fa1
// Place a wall at d3 : Sd3
// Place a Capstone at b4 : Cb4
// Move 5 stones from e4, dropping two on d4 and three on c4 : 5e4<23
string Move::to_string() const {
  string m = "";

  // Append coordinated
  m += (char)('a' + pos % size);
  m += (char)('1' + pos / size);

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

  // Done if place move
  if(is_place()) return m;

  // Moves are stored in locations 0xf0, 0xf00...
  // Location 0xf has the number of drops
  // Example 2,3 -> (3)(2)(2)
  // Example 3,1,1,1 -> (1)(1)(1)(3)(4)
  int sum = 0;
  for(int i=0; i<num_slide(); ++i){
    sum += slide_at(i);
    m += (char)('0' + slide_at(i));
  }
  m = (char)('0' + sum) + m;
  return m;
}

} // namespace Tak