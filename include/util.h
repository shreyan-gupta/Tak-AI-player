#ifndef UTIL_H
#define UTIL_H

#include <vector>

namespace Tak {

using namespace std;
using Bit = uint64_t;
using s_int = uint8_t;

// Global variables
extern int size;

namespace SlideVec {
  extern vector<vector<vector<Bit>>> cap_slides;
  extern vector<vector<vector<Bit>>> all_slides;
}

namespace Bits {
  extern Bit Mask;
  extern Bit L;
  extern Bit R;
  extern Bit U;
  extern Bit D;
} // namespace Bit

// init function
void init(int board_size);

enum class Player {
  Black,
  White
};

struct Pieces {
  s_int black_flat;
  s_int white_flat;
  s_int black_cap;
  s_int white_cap;
};


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

inline bool has_bit(Bit &b, s_int pos){
  return ((b & (1 << pos)) != 0);
}

inline void add_bit(Bit &b, s_int pos){
  b |= (1 << pos);
}

inline void remove_bit(Bit &b, s_int pos){
  b &= ~(1 << pos);
}

} // namespace Tak

#endif