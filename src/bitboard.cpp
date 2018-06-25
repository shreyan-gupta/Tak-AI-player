#include <iostream>

#include "bitboard.h"

namespace Tak {

using namespace std;

inline bool has_bit(Bit &b, s_int pos){
  return ((b & (1 << pos)) != 0);
}

inline void add_bit(Bit &b, s_int pos){
  b |= (1 << pos);
}

inline void remove_bit(Bit &b, s_int pos){
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
    b >>= 1;
    ++gap;
  }
  return gap;
}

inline bool is_last_part_one(Bit b){
  while((b & ((Bit)(-1) - ((Bit)(-1)>>1))) == 0) b <<= 1;
  b <<= 1;
  return ((b & ((Bit)(-1) - ((Bit)(-1)>>1))) == 0);
}

inline void switch_player(Player &player){
  if(player == Player::Black) player = Player::White;
  else player = Player::Black;
}

s_int BitBoard::board_size = 5;

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


// Check if a move is valid
// Assert checks are always suppose to be true
// Other checks may return false
bool BitBoard::is_valid_move(Move &move){
  auto pos = move.pos;
  
  // Check if valid move position
  assert(pos < board_size*board_size);
  
  if(move.is_place()){
    if(height[pos] != 0) return false;
  }
  else { // move.is_slide()
    // atleast 1 element
    assert(move.slide != 0);
    // less than carry_limit pieces
    assert(count_bit(move.slide) <= board_size);
    // atleast sum_slide number of pieces in stack
    assert(count_bit(move.slide) <= height[pos]);
    
    // Boundary asserts
    switch(move.move_type){
      case MoveType::SlideLeft  : assert(pos%board_size >= count_part(move.slide)); break; 
      case MoveType::SlideRight : assert(pos%board_size + count_part(move.slide) < board_size); break; 
      case MoveType::SlideUp    : assert(pos + board_size*count_part(move.slide) < board_size*board_size); break; 
      case MoveType::SlideDown  : assert(pos > board_size*count_part(move.slide)); break; 
      default : assert(false);
    }

    // if slide move, then ownership with current player
    if(current_player == Player::Black && !has_bit(black_stones, pos)) return false;
    if(current_player == Player::White && !has_bit(white_stones, pos)) return false;

    int dpos = move.get_dpos();
    int num_part = count_part(move.slide);

    move.cap_move = 
      has_bit(cap_stones, pos) &&
      has_bit(wall_stones, (s_int)(pos + num_part*dpos)) &&
      is_last_part_one(move.slide);

    // Either a cap move or there should be no walls
    for(int i=0; i<num_part; ++i){
      pos += num_part;
      if(has_bit(wall_stones, pos)){
        // If not cap move, then invalid move
        // else i should be the last slide
        if(!move.cap_move) return false;
        else if(i != num_part-1) return false;
      }
    }
  }
  return true;
}

// Play move on bitboard
// Changes the required states
void BitBoard::play_move(Move &move){
  auto pos = move.pos;

  // Place move
  if(move.is_place()){
    // update height to 1
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

    // Direction of slide
    int dpos = move.get_dpos();
    int num_part = count_part(move.slide);

    auto &curr_piece = pieces[pos];
    auto &curr_height = height[pos];
    auto temp_slide = move.slide;
    auto old_pos = pos;

    // Start with last pos
    pos += dpos*num_part;

    // Decide whether cap move
    move.cap_move = 
      has_bit(cap_stones, old_pos) &&
      has_bit(wall_stones, pos) &&
      is_last_part_one(move.slide);

    // If cap_move
    if(move.cap_move){
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
    if(current_player == Player::White)
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
    int dpos = move.get_dpos();

    auto &curr_piece = pieces[pos];
    auto &curr_height = height[pos];
    auto temp_slide = move.slide;
    auto old_pos = pos;

    // Iterate in forward direction
    while(temp_slide != 0){
      // Update position
      pos += dpos;

      // Get rid of the 0s
      while((temp_slide & ((Bit)(-1) - ((Bit)(-1)>>1))) == 0) temp_slide <<= 1;
      int i = 0;
      // Increment till we see 1
      while((temp_slide & ((Bit)(-1) - ((Bit)(-1)>>1))) != 0){
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

// For debugging
void BitBoard::print(){
  int board_size = 5;
  cout << "Board size " << board_size << endl;
  cout << ((current_player == Player::Black)? "Black":"White") << " player playing" << endl;
  for(int y=board_size-1; y>=0; --y){
    cout << y+1;
    for(int x=0; x<board_size; ++x){
      cout << "\t";
      s_int i = x + board_size*y;
      if(height[i] == 0) continue;
      for(s_int h=height[i]-1; h>0; --h){
        bool is_white = (pieces[i] >> h) & 1;
        cout << (is_white?"F":"f");
      }
      bool is_white = pieces[i] & 1;
      if(has_bit(wall_stones, i)) cout << (is_white?"S":"s");
      else if(has_bit(cap_stones, i)) cout << (is_white?"C":"c");
      else cout << (is_white?"F":"f");
    }
    cout << endl;
  }
  for(int x=0; x<board_size; ++x){
    cout << "\t" << (char)('a' + x);
  }
  cout << endl;
}

} // namespace Tak