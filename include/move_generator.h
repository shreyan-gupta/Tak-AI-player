#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <vector>

namespace Tak {

using Bit = uint64_t;
using s_int = uint8_t;

class Move;
class BitBoard;
enum class MoveType;

class MoveGenerator {
 public:
  MoveGenerator(BitBoard &board);
  bool has_next() {return !done;}
  const Move& next();
 
 private:
  BitBoard &board;
  Move move;
  bool done;

  // All states required
  int i;
  int j;
  int k;
  int STATE;
  Bit STATE_slides;
  Bit STATE_curr_stones;
  vector<MoveType> STATE_move_type;
  vector<Bit> *STATE_slide_vec;

  get_max_slide(s_int pos);
};

} // namespace Tak

#endif