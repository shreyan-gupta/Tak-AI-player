#include <algorithm>
#include <cmath>

#include "util.h"
#include "tak/feature.h"

namespace Tak {

using namespace Weights;

// Given a seed, grows the seed by one place
// to left, right, up, down
Bit grow(Bit seed, Bit within) {
  Bit next = seed;
  next |= (seed << 1) & ~Bits::L;
  next |= (seed >> 1) & ~Bits::R;
  next |= (seed << size);
  next |= (seed >> size);
  return next & within;
}

// Gets connected component given seed
Bit flood(Bit seed, Bit within) {
  Bit next;
  while(true){
    next = grow(seed, within);
    if(next == seed) return seed;
    seed = next;
  }
}

// First calculate the width and height of group
// Then calculate group score
eval_t get_group_score(Bit g) {
  int width = 0;
  int height = 0;

  // Move wall from left to right
  Bit wall = Bits::L;
  while((g & wall) == 0) wall <<= 1;
  while((g & wall) != 0){
    ++width;
    if(wall == Bits::R) break;
    wall <<= 1;
  }

  // Move wall from bottom to top
  wall = Bits::D;
  while((g & wall) == 0) wall <<= size;
  while((g & wall) != 0){
    ++height;
    wall <<= size;
  }
  assert(width <= size);
  assert(height <= size);
  return GROUPS[width] + GROUPS[height];
}

eval_t Feature::operator()(const BitBoard &b) {
  
  // Multiplier is set of 1 if wrt white
  int multiplier = is_white(b.current_player)? 1 : -1;
  set_delta(b);

  // Flatwin
  // If either player runs out of pieces, or if the board 
  // is covered, then the player with the most flatstones wins.
  if(
    b.pieces.num_black() == 0 ||
    b.pieces.num_white() == 0 ||
    (b.black_stones | b.white_stones) == Bits::Mask
  ) {
    if(delta_flat > 0) return multiplier * WIN;
    else if(delta_flat < 0) return multiplier * (-WIN);
  }

  eval_t white_group = score_groups(b.white_stones & ~b.wall_stones);
  eval_t black_group = score_groups(b.black_stones & ~b.wall_stones);
  
  // Double Win
  // In case both black and white manage to make a road
  // the current player wins
  if(white_group >= WIN && black_group >= WIN) return WIN;
  
  // Iterate over all position to get captive score
  eval_t score = 0;
  for(int i=0; i<size*size; ++i){
    if(b.height[i] <= 1) continue;
    if(test_bit(b.wall_stones, i))
      score += score_captive(b.stack[i], b.height[i], HARD_SCAPTIVE, SOFT_SCAPTIVE);
    else if(test_bit(b.cap_stones, i))
      score += score_captive(b.stack[i], b.height[i], HARD_CCAPTIVE, SOFT_CCAPTIVE);
    else
      score += score_captive(b.stack[i], b.height[i], HARD_FCAPTIVE, SOFT_FCAPTIVE);
  }

  // Add all other feature scores
  score += score_top_piece(b);
  score += score_center(b.white_stones);
  score -= score_center(b.black_stones);
  score += white_group;
  score -= black_group;

  return multiplier * score;
}

// Calculate score for all groups given a base
// A base is embedding of all flat and cap stones
eval_t Feature::score_groups(Bit base) {
  eval_t score = 0;
  while(base != 0){
    Bit bit = base & (-base);
    Bit group = flood(bit, base);
    score += get_group_score(group);
    base &= ~group;
  }
  return score;
}

// Calculate the captive scores for a given position stack
// Hard captive are player's pieces, soft captive are opponent's
// Negate value if owned by black
eval_t Feature::score_captive(uint32_t stack, s_int height, eval_t hard, eval_t soft) {
  bool owner = stack & 1;
  int my_captive = 0;
  int their_captive = 0;
  
  for(int i=1; i<height; ++i){
    stack >>= 1;
    if((stack & 1) == owner) ++my_captive;
    else ++their_captive;
  }
  
  eval_t score = my_captive * hard + their_captive * soft;
  if(owner) return score;
  else return -score;
}

// Score of the top most pieces
// Endgame cutoff works by increasing weight of flat
// piece when close to the end
// weight = ENDGAMEFLAT * (1 - min(b,w,c)/c)
eval_t Feature::score_top_piece(const BitBoard &b){
  // Assume that set_delta function is already called
  eval_t score = delta_flat*FLAT + delta_wall*WALL + delta_cap*CAP;
  float min_left = std::min(b.pieces.num_black(), b.pieces.num_white());
  score += (1 - std::min(min_left, ENDGAMECUTOFF)/ENDGAMECUTOFF)*delta_flat*ENDGAMEFLAT;
  return score;
}

// A position closer to the center is preferred
// calculated as -vs of distance from center
eval_t Feature::score_center(Bit base) {
  eval_t score = 0;
  Bit h_wall = Bits::L;
  Bit v_wall = Bits::D;
  for(int i=0; i<size; ++i){
    int p = popcnt(base & h_wall) + popcnt(base & v_wall);
    score -= CENTER * p * std::abs(2*i - (size-1));
    h_wall <<= 1;
    v_wall <<= size;
  }
  return score;
}

// Sets the value of delta_flat, delta_wall, delta_cap
void Feature::set_delta(const BitBoard &b) {
  delta_wall = popcnt(b.wall_stones & b.white_stones) - popcnt(b.wall_stones & b.black_stones);
  delta_cap = b.pieces.black_cap - b.pieces.white_cap;
  delta_flat = (popcnt(b.white_stones) - popcnt(b.black_stones)) - delta_wall - delta_cap;
}

} // namespace Tak