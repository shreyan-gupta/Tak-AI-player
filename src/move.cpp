#include "move.h"

namespace Tak {

s_int Move::board_size = 5;

Move::Move(string move){  
  pos = (move[1] - 'a') + board_size*(move[2] - '1');

  if(move[0] == 'F') move_type = MoveType::PlaceFlat;
  else if(move[0] == 'S') move_type = MoveType::PlaceWall;
  else if(move[0] == 'C') move_type = MoveType::PlaceCapstone;
  else if(move[3] == '<') move_type = MoveType::SlideLeft;
  else if(move[3] == '>') move_type = MoveType::SlideRight;
  else if(move[3] == '+') move_type = MoveType::SlideUp;
  else if(move[3] == '-') move_type = MoveType::SlideDown;

  if(is_slide()){
    for(int i=4; i<move.size(); ++i){
      int shift = move[i] - '0';
      slide <<= (shift+1);
      slide |= ((1 << shift) - 1);
    }
  }
}

Move::Move(MoveType move_type, s_int pos) :
  move_type(move_type),
  pos(pos),
  cap_move(false),
  slide(0) {
  assert(move_type < MoveType::SlideLeft);
}

bool Move::operator==(const Move &rhs){
  return(
    move_type == rhs.move_type &&
    pos == rhs.pos &&
    slide == rhs.slide
  );
}

Move::Move(MoveType move_type, s_int pos, Bit slide) :
  move_type(move_type),
  pos(pos),
  cap_move(false),
  slide(slide) {
  assert(move_type >= MoveType::SlideLeft);
}

int Move::get_dpos(){
  switch(move_type){
    case MoveType::SlideLeft  : return -1;
    case MoveType::SlideRight : return 1;
    case MoveType::SlideUp    : return board_size;
    case MoveType::SlideDown  : return -board_size;
    default : assert(false);
  }
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