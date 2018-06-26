#ifndef UTIL_H
#define UTIL_H

#include <vector>

namespace Tak {

using namespace std;
using Bit = uint64_t;
using s_int = uint8_t;

// enum Player
enum class Player {
  Black,
  White
};

// Contains information about pieces left
// for each player
struct Pieces {
  s_int black_flat;
  s_int white_flat;
  s_int black_cap;
  s_int white_cap;
};

// Global variables
extern int size;
extern Pieces default_pieces;

namespace SlideVec {
  extern vector<vector<vector<Bit>>> cap_slides;
  extern vector<vector<vector<Bit>>> all_slides;
} // namespace SlideVec

namespace Bits {
  extern Bit Mask;
  extern Bit L;
  extern Bit R;
  extern Bit U;
  extern Bit D;
} // namespace Bit

namespace InitPieces {
  extern Pieces init5;
  extern Pieces init6;
  extern Pieces init7;
  extern Pieces init_default;
} // namespace InitPieces

// init function
void init(int board_size);

inline bool is_black(Player &p){
  return p == Player::Black;
}

inline bool is_white(Player &p){
  return !is_black(p);
}

inline void switch_player(Player &p){
  if(is_black(p)) p = Player::White;
  else p = Player::Black;
}

inline bool test_bit(Bit b, s_int pos){
  return (b >> pos) & 1; 
}

inline void set_bit(Bit &b, s_int pos){
  b |= (1 << pos);
}

inline void reset_bit(Bit &b, s_int pos){
  b &= ~(1 << pos);
}

} // namespace Tak

#endif