#include "bitboard.h"

namespace Tak {

inline bool has_bit(Bit &b, s_int &pos){
  return ((b & (1 << pos)) != 0);
}

inline void add_bit(Bit &b, s_int &pos){
  b |= (1 << pos);
}

inline void remove_bit(Bit &b, s_int &pos){
  if((b & (1 << pos)) != 0) b ^= (1 << pos);
}

inline s_int count_bit(Bit b){
  s_int sum = 0;
  while(b != 0){
    sum += (b & 1);
    b >>= 1;
  }
  return sum;
}

inline s_int count_part(Bit b){
  s_int gap = 0;
  while(b != 0){
    assert(b & 1);
    while(b & 1) b >>= 1;
    ++gap;
  }
  return gap;
}

inline bool is_last_part_one(Bit b){
  while((b & ((-1) - ((-1)>>1))) == 0) b <<= 1;
  b <<= 1;
  return ((b & ((-1) - ((-1)>>1))) == 0);
}

inline switch_player(Player &player){
  if(player == Player::Black) player = Player:White;
  else player = Player::Black;
}


// 0 -> Black
// 1 -> White
BitBoard::BitBoard() :
  current_player(Player::White),
  black_stones(0),
  white_stones(0),
  wall_stones(0),
  cap_stones(0),
  height(vector<s_int>(board_size*board_size, 0)),
  pieces(vector<s_int>(board_size*board_size, 0)) {}

// Play move on bitboard
// Changes the required states
void BitBoard::play_move(Move &move){
  auto pos = move.pos;
  
  // Check if valid move position
  assert(pos < board_size*board_size);

  // Place move
  if(move.is_place()){
    // if place move, then position should be empty
    assert(height[pos] == 0);
    height[pos] = 1;
    
    // update stones board and pieces array
    if(current_player == Player::Black)
      add_bit(black_stones, pos);
    else {
      add_bit(white_stones, pos);
      pieces[pos] = 1;
    }

    // update wall and cap stones
    if(move.move_type == MoveType::PlaceWall)
      add_bit(wall_stones, pos);
    if(move.move_type == MoveType::PlaceCapstone)
      add_bit(cap_stones, pos);
  }

  if(move.is_slide()){
    // atleast 1 element
    assert(move.slide != 0);
    // less than carry_limit pieces
    assert(count_bit(move.slide) <= board_size);
    // atleast sum_slide number of pieces in stack
    assert(count_bit(move.slide) <= height[pos]);
    // if slide move, then ownership with current player
    if(current_player == Player::Black){
      assert(has_bit(black_stones, pos));
    }
    else{
      assert(has_bit(white_stones, pos));
    }
    // Boundary asserts
    switch(move.move_type){
      case MoveType::SlideLeft  : assert(pos%board_size >= count_part(move.slide)); break; 
      case MoveType::SlideRight : assert(pos%board_size + count_part(move.slide) < board_size); break; 
      case MoveType::SlideUp    : assert(pos + board_size*count_part(move.slide) < board_size*board_size); break; 
      case MoveType::SlideDown  : assert(pos > board_size*count_part(move.slide)); break; 
      default : assert(false);
    }

    // Direction of slide
    int dpos;
    switch(move.move_type){
      case MoveType::SlideLeft  : dpos = -1; break; 
      case MoveType::SlideRight : dpos = 1; break; 
      case MoveType::SlideUp    : dpos = board_size; break; 
      case MoveType::SlideDown  : dpos = -board_size; break; 
      default : assert(false);
    }

    auto &curr_piece = pieces[pos];
    auto &curr_height = height[pos];
    auto temp_slide = move.slide;
    auto old_pos = pos;

    // Start with last pos
    pos += dpos*count_part(move.slide);

    // If cap_move
    if(move.cap_move){
      assert(is_last_part_one(move.slide));
      assert(has_bit(wall_stones, pos));
      assert(has_bit(cap_stones, old_pos));
      remove_bit(wall_stones, pos);
    }

    if(has_bit(cap_stones, old_pos)){
      remove_bit(cap_stones, old_pos);
      add_bit(cap_stones, pos);
    }

    if(has_bit(wall_stones, old_pos)){
      remove_bit(wall_stones, old_pos);
      add_bit(wall_stones, pos);
    }
    
    // iterate the slide
    while(temp_slide != 0){
      int i = 0;
      assert(temp_slide & 1);
      while(temp_slide & 1){
        ++i;
        temp_slide >>= 1;
      }
      temp_slide >>= 1;

      // Should not be a wall
      assert(!has_bit(wall_stones, pos));

      // Update heiht and pieces at pos and old_pos
      height[pos] += i;
      pieces[pos] <<= i;
      pieces[pos] |= (curr_piece & ((1 << i) - 1));
      curr_piece >>= i;
      curr_height -= i;

      // Update black and white stones
      if(pieces[pos] & 1){
        add_bit(white_stones, pos);
        remove_bit(black_stones, pos);
      }
      else {
        add_bit(black_stones, pos);
        remove_bit(white_stones, pos);
      }
      
      // Update pos
      pos -= dpos;
    }
    // Final pos should be same as old_pos
    assert(pos == old_pos);
  }

  // Switch current player
  switch_player(current_player);
}

// Undo a move played on the board
// We assume this is the state we have reached after play_move
void BitBoard::undo_move(Move &move){
  auto pos = move.pos;
  
  if(move.is_place()){
    height[pos] = 0;
    pieces[pos] = 0;
    if(current_player == Place::White)
      remove_bit(black_stones, pos);
    else
      remove_bit(white_stones, pos);
    if(move.move_type == MoveType::PlaceWall)
      remove_bit(wall_stones, pos);
    if(move.move_type == MoveType::PlaceCapstone)
      remove_bit(cap_stones, pos);
  }

  if(move.is_slide()){
    // Get delta pos
    int dpos;
    switch(move.move_type){
      case MoveType::SlideLeft  : dpos = -1; break; 
      case MoveType::SlideRight : dpos = 1; break; 
      case MoveType::SlideUp    : dpos = board_size; break; 
      case MoveType::SlideDown  : dpos = -board_size; break; 
      default : assert(false);
    }

    auto &curr_piece = pieces[pos];
    auto &curr_height = height[pos];
    auto temp_slide = move.slide;
    auto old_pos = pos;

    // Iterate in forward direction
    while(temp_slide != 0){
      // Get rid of the 0s
      while((temp_slide & ((-1) - ((-1)>>1))) == 0) temp_slide <<= 1;
      int i = 0;
      // Increment till we see 1
      while((temp_slide & ((-1) - ((-1)>>1))) != 0){
        ++i;
        temp_slide <<= 1;
      }

      curr_height += i;
      curr_piece <<= i;
      curr_piece |= (pieces[pos] & ((1 << i) - 1));
      height[pos] -= i;
      pieces[pos] >>= i;

      if(pieces[pos] & 1){
        add_bit(white_stones, pos);
        remove_bit(black_stones, pos);
      }
      else {
        add_bit(black_stones, pos);
        remove_bit(white_stones, pos);
      }

      pos += dpos;
    }

    if(move.cap_move){
      add_bit(wall_stones, pos);
    }

    if(has_bit(cap_stones, pos)){
      add_bit(cap_stones, old_pos);
      remove_bit(cap_stones, pos);
    }

    if(has_bit(wall_stones, pos)){
      add_bit(wall_stones, old_pos);
      remove_bit(wall_stones, pos);
    }
  }

  // Switch current player
  switch_player(current_player);
}

} // namespace Tak