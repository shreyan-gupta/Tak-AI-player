#include <algorithm>

#include "tak/util.h"
#include "tak/bitboard.h"
#include "tak/move_generator.h"

// Jugaad for coroutine. To be used as shown below
// crBegin;
// for(int i=0; i<n; ++i){
//   crReturn(i);
// }
// crFinish;
#define crBegin switch(STATE) { case 0:
#define crReturn(x) do { STATE=__LINE__; return x; case __LINE__:; } while (0)
#define crFinish STATE=-1; break; case -1: assert(false); }

namespace Tak {

MoveGenerator::MoveGenerator(BitBoard &board) : 
  board(board),
  STATE(0) {}

bool MoveGenerator::has_next() {
  // begin coroutine
  crBegin;

  // find if current_player has flats or caps
  bool has_flat;
  bool has_cap;
  if(is_black(board.current_player)) {
    STATE_curr_stones = board.black_stones;
    has_flat = board.pieces.black_flat != 0;
    has_cap = board.pieces.black_cap != 0;
  }
  else {
    STATE_curr_stones = board.white_stones;
    has_flat = board.pieces.white_flat != 0;
    has_cap = board.pieces.white_cap != 0;
  }

  // Figure out which all places moves are valid
  if(has_flat){
    STATE_move_type.push_back(MoveType::PlaceFlat);
    STATE_move_type.push_back(MoveType::PlaceWall);
  }
  if(has_cap){
    STATE_move_type.push_back(MoveType::PlaceCapstone);
  }

  move.cap_move = false;
  move.slide = 0;

  // Generate place moves
  for(i=0; i<board.height.size(); ++i){
    if(board.height[i] != 0) continue;
    // Set move pos
    move.pos = i;
    for(j=0; j<STATE_move_type.size(); ++j){
      // Set move_type
      move.move_type = STATE_move_type[j];
      crReturn(true);
    }
  }

  // Should be in reverse order as the get_max_slide function
  STATE_move_type = {
    MoveType::SlideDown,
    MoveType::SlideUp, 
    MoveType::SlideRight, 
    MoveType::SlideLeft, 
  };

  // Generate slide moves
  for(i=0; i<board.height.size(); ++i){
    if(!test_bit(STATE_curr_stones, i)) continue;

    // set move pos
    // get informations about slides
    // If not a cap_stone at pos i, then reset capable
    move.pos = i;
    STATE_slides = get_max_slide(i);
    if(!test_bit(board.cap_stones, i)) STATE_slides &= ~0xf;

    // Iterate over all the slide moves
    for(j=0; j<STATE_move_type.size(); ++j){
      // Set the move type
      move.move_type = STATE_move_type[j];

      // Get the value of num_drops and num_pieces
      STATE_num_pieces = std::min((int)board.height[i], size);
      STATE_num_drops = (STATE_slides >> (4*j + 4)) & 0xf;
      // If we can't reach cap_move wall, unset cap_move
      if(STATE_num_pieces < STATE_num_drops+1) reset_bit(STATE_slides, j);
      STATE_num_drops = std::min(STATE_num_drops, STATE_num_pieces);
      
      // Get pointer to appropriate slide_vec
      // Iterate over all slide values
      STATE_slide_vec = &SlideVec::all_slides[STATE_num_drops][STATE_num_pieces];
      for(k=0; k<STATE_slide_vec->size(); ++k){
        move.slide = (*STATE_slide_vec)[k];
        crReturn(true);
      }

      // If cap bit is not set, continue
      if(!test_bit(STATE_slides, j)) continue;

      // Set cap_move
      move.cap_move = true;
      // Get pointer to appropriate slide_vec for cap moves
      STATE_slide_vec = &SlideVec::cap_slides[STATE_num_drops+1][STATE_num_pieces];
      // Iterate over all slide values
      for(k=0; k<STATE_slide_vec->size(); ++k){
        move.slide = (*STATE_slide_vec)[k];
        crReturn(true);
      }
      // Unset cap_move
      move.cap_move = false;
    }
  }
  // finish coroutine
  crFinish;
  return false;
}

// Returns the slides possible for each direction
// ret & 0xf gives the 4 cap_able bits
// (ret & 0xf0) to (ret & 0xf0000) gives the max slide values
// for directions L, R, U, D respectively
Bit MoveGenerator::get_max_slide(int pos){
  Bit capable = 0;
  Bit slides = 0;

  // In the sequence left, right, up, down
  // boundaries are shifted by 1 position 
  int directions[] = {-1, +1, size, -size};
  Bit boundaries[] = {Bits::R, (Bits::R << 1), (Bits::U << size), 0};

  for(int i=0; i<4; ++i){
    capable <<= 1;
    int temp_pos = pos;
    Bit forbidden = board.wall_stones | board.cap_stones | boundaries[i];
    temp_pos += directions[i];
    
    // temp_pos should not go out of bounds and not touch forbidden
    while(temp_pos >= 0 && !test_bit(forbidden, temp_pos)){
      temp_pos += directions[i];
      ++slides;
    }

    // If not on boundary but wall, then capable
    bool on_boundary = temp_pos < 0 || test_bit(boundaries[i], temp_pos);
    if(!on_boundary && test_bit(board.wall_stones, temp_pos)) ++capable;
    slides <<= 4;
  }
  return slides | capable;
}

} // namespace Tak