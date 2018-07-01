#include <iostream>

#include "tak/util.h"
#include "tak/bitboard.h"

using namespace std;

namespace Tak {

// Black is represented by bit 0
// White is represented by bit 1
BitBoard::BitBoard(Pieces pieces) :
  current_player(Player::White),
  pieces(pieces),
  black_stones(0),
  white_stones(0),
  wall_stones(0),
  cap_stones(0),
  height(std::vector<s_int>(size*size, 0)),
  stack(std::vector<uint32_t>(size*size, 0)) {}


// Check if a move is valid
// Assert checks are always suppose to be true
// Other checks may return false
// Also sets the value of cap_move
bool BitBoard::is_valid_move(Move &move) const {
  if(move.move_type == MoveType::Invalid) return false;
  int pos = move.pos;
  
  // Check if valid move position
  assert(pos < size*size);
  
  if(move.is_place()){
    if(height[pos] != 0) return false;
    
    auto &curr_cap = is_black(current_player)? pieces.black_cap: pieces.white_cap;
    auto &curr_flat = is_black(current_player)? pieces.black_flat: pieces.white_flat;
    
    // There should be enough pieces to play move
    switch(move.move_type) {
      case MoveType::PlaceFlat : if(curr_flat == 0) return false; break;
      case MoveType::PlaceWall : if(curr_flat == 0) return false; break;
      case MoveType::PlaceCapstone : if(curr_cap == 0) return false; break;
      default : assert(false);
    }

    assert(stack[pos] == 0);
    assert(!test_bit(black_stones, pos));
    assert(!test_bit(white_stones, pos));
    assert(!test_bit(wall_stones, pos));
    assert(!test_bit(cap_stones, pos));
  }

  if(move.is_slide()) {
    int num_slide = move.num_slide();
    int dpos = move.get_dpos();
    
    // num_stack less than number of pieces in stack
    if(move.num_stack() > height[pos]) return false;
    
    // num_stack less than carry_limit stack
    assert(move.num_stack() <= size);
    // atleast 1 element
    assert(num_slide > 0);
    // Boundary checks
    switch(move.move_type){
      case MoveType::SlideLeft  : if(pos%size < num_slide) return false; break;
      case MoveType::SlideRight : if(pos%size + num_slide >= size) return false; break;
      case MoveType::SlideUp    : if(pos + size * num_slide >= size*size) return false; break;
      case MoveType::SlideDown  : if(pos < size * num_slide) return false; break;
      default : assert(false);
    }

    // if slide move, then ownership with current player
    if(is_black(current_player) && !test_bit(black_stones, pos)) return false;
    if(is_white(current_player) && !test_bit(white_stones, pos)) return false;

    // Set the value of cap_move
    move.cap_move = 
      test_bit(cap_stones, pos) &&
      test_bit(wall_stones, pos + num_slide*move.get_dpos()) &&
      (move.slide_at(num_slide - 1) == 1);

    // Either a cap move or there should be no walls
    for(int i=0; i<num_slide; ++i){
      assert(move.slide_at(i) > 0);
      pos += dpos;
      if(test_bit(cap_stones, pos)) return false;
      if(test_bit(wall_stones, pos)){
        // If not cap move, then invalid move
        // else i should be the last slide
        if(!move.cap_move) return false;
        else if(i != num_slide-1) return false;
      }
    }
  }
  return true;
}

// Play move on bitboard
// Changes the required states
void BitBoard::play_move(const Move &move){
  auto pos = move.pos;

  // Place move
  if(move.is_place()){
    // update height to 1
    height[pos] = 1;

    auto &curr_cap = is_black(current_player)? pieces.black_cap: pieces.white_cap;
    auto &curr_flat = is_black(current_player)? pieces.black_flat: pieces.white_flat;
    
    // Set number of stones left
    switch(move.move_type) {
      case MoveType::PlaceFlat : --curr_flat; break;
      case MoveType::PlaceWall : --curr_flat; set_bit(wall_stones, pos); break;
      case MoveType::PlaceCapstone : --curr_cap; set_bit(cap_stones, pos); break;
      default : assert(false);
    }

    // update stones board
    if(is_black(current_player)){
      set_bit(black_stones, pos);
    }
    else {
      set_bit(white_stones, pos);
      stack[pos] = 1;
    }
  }

  // Slide move
  if(move.is_slide()){

    // Direction of slide, number of parts and end position
    int dpos = move.get_dpos();
    int num_part = move.num_slide();
    int end_pos = pos + dpos*num_part;

    // If cap_move
    if(move.cap_move){
      reset_bit(wall_stones, end_pos);
    }

    // If capstone moved
    if(test_bit(cap_stones, pos)){
      reset_bit(cap_stones, pos);
      set_bit(cap_stones, end_pos);
    }

    // If wall moved
    if(test_bit(wall_stones, pos)){
      reset_bit(wall_stones, pos);
      set_bit(wall_stones, end_pos);
    }

    for(int i=num_part-1; i>=0; --i){
      int d = move.slide_at(i);
      int curr_pos = pos + dpos*(i+1);

      // Update height and stack at pos and curr_pos
      height[curr_pos] += d;
      stack[curr_pos] <<= d;
      stack[curr_pos] |= (stack[pos] & ((1 << d) - 1));
      stack[pos] >>= d;
      height[pos] -= d;

      // Update black and white stones at curr_pos
      set_player_at_pos(curr_pos);
    }
    // Update black and white stones at pos
    set_player_at_pos(pos);
  }
  // Switch current player
  switch_player(current_player);
}

// Undo a move played on the board
// We assume this is the state we have reached after play_move
void BitBoard::undo_move(const Move &move){
  auto pos = move.pos;
  
  // Place move
  if(move.is_place()){
    // Set stack and height to 0
    height[pos] = 0;
    stack[pos] = 0;

    reset_bit(black_stones, pos);
    reset_bit(white_stones, pos);
    reset_bit(wall_stones, pos);
    reset_bit(cap_stones, pos);
    
    auto &opp_cap = is_white(current_player)? pieces.black_cap: pieces.white_cap;
    auto &opp_flat = is_white(current_player)? pieces.black_flat: pieces.white_flat;
    
    // Reset number of stones left
    switch(move.move_type) {
      case MoveType::PlaceFlat : ++opp_flat; break;
      case MoveType::PlaceWall : ++opp_flat; break;
      case MoveType::PlaceCapstone : ++opp_cap; break;
      default : assert(false);
    }
  }

  // Slide move
  if(move.is_slide()){

    // Direction of slide, number of parts and end position
    int dpos = move.get_dpos();
    int num_part = move.num_slide();
    int end_pos = pos + dpos*num_part;

    // If cap_move
    if(move.cap_move){
      set_bit(wall_stones, end_pos);
    }

    // If capstone moved
    if(test_bit(cap_stones, end_pos)){
      reset_bit(cap_stones, end_pos);
      set_bit(cap_stones, pos);
    }

    // If wall moved
    // Move must not be a cap move
    if(test_bit(wall_stones, end_pos) && !move.cap_move){
      reset_bit(wall_stones, end_pos);
      set_bit(wall_stones, pos);
    }

    for(int i=0; i<num_part; ++i){
      int d = move.slide_at(i);
      int curr_pos = pos + dpos*(i+1);

      // Update height and stack at pos and curr_pos
      height[pos] += d;
      stack[pos] <<= d;
      stack[pos] |= (stack[curr_pos] & ((1 << d) - 1));
      stack[curr_pos] >>= d;
      height[curr_pos] -= d;

      // Update black and white stones at curr_pos
      set_player_at_pos(curr_pos);
    }
    // Update black and white stones at pos
    set_player_at_pos(pos);
  }
  // Switch current player
  switch_player(current_player);
}

// Hash function for BitBoard
size_t BitBoard::hash() const {
  size_t seed = black_stones + wall_stones;
  for(int i=0; i<height.size(); ++i){
    seed ^= 0x9e3779b9 + stack[i] + height[i] + (seed << 3) + (seed >> 5);
  }
  seed ^= white_stones + cap_stones;
  if(is_black(current_player)) return seed;
  else return ~seed;
}

// operator==
bool BitBoard::operator==(const BitBoard &rhs) const {
  if(
    black_stones != rhs.black_stones ||
    white_stones != rhs.white_stones ||
    wall_stones != rhs.wall_stones ||
    cap_stones != rhs.cap_stones ||
    current_player != rhs.current_player
  ) return false;;

  for(int i=0; i<height.size(); ++i){
    if(
      height[i] != rhs.height[i] ||
      stack[i] != rhs.stack[i]
    ) return false;
  }
  return true;
}

// Check if the board is in a valid state
// Used for debugging
bool BitBoard::is_valid() const {
  auto bw_stones = black_stones | white_stones;
  assert((black_stones & white_stones) == 0);
  assert((wall_stones & ~bw_stones) == 0);
  assert((cap_stones & ~bw_stones) == 0);

  for(int i=0; i<size*size; ++i){
    if(height[i] == 0){
      assert(stack[i] == 0);
      assert(!test_bit(black_stones, i));
      assert(!test_bit(white_stones, i));
      assert(!test_bit(wall_stones, i));
      assert(!test_bit(cap_stones, i));
    }
    else {
      assert((stack[i] >> height[i]) == 0);
      if(stack[i] & 1) assert(test_bit(white_stones, i));
      else assert(test_bit(black_stones, i));
    }
  }
  return true;
}

// For debugging
void BitBoard::print() const {
  cout << "Board size " << size << endl;
  cout << ((current_player == Player::Black)? "Black":"White") << " player playing" << endl;
  for(int y=size-1; y>=0; --y){
    cout << y+1;
    for(int x=0; x<size; ++x){
      cout << "\t";
      s_int i = x + size*y;
      if(height[i] == 0) continue;
      for(s_int h=height[i]-1; h>0; --h){
        bool is_white = (stack[i] >> h) & 1;
        cout << (is_white?"F":"f");
      }
      bool is_white = stack[i] & 1;
      if(test_bit(wall_stones, i)) cout << (is_white?"S":"s");
      else if(test_bit(cap_stones, i)) cout << (is_white?"C":"c");
      else cout << (is_white?"F":"f");
    }
    cout << endl;
  }
  for(int x=0; x<size; ++x){
    cout << "\t" << (char)('a' + x);
  }
  cout << endl;
}

// Sets position ownership at location 'pos'
void BitBoard::set_player_at_pos(s_int pos){
  if(height[pos] == 0){
    reset_bit(black_stones, pos);
    reset_bit(white_stones, pos);
  }
  else if(stack[pos] & 1){
    set_bit(white_stones, pos);
    reset_bit(black_stones, pos);
  }
  else {
    set_bit(black_stones, pos);
    reset_bit(white_stones, pos);
  }
}

} // namespace Tak