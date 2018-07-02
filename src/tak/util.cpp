#include <random>

#include "tak/util.h"

namespace Tak {

using namespace std;
using namespace SlideVec;

// Global variables
int size;
Pieces default_pieces;

namespace SlideVec {
  vector<vector<vector<Bit>>> cap_slides;
  vector<vector<vector<Bit>>> all_slides;
} // namespace SlideVec

namespace Bits {
  Bit Mask;
  Bit L;
  Bit R;
  Bit U;
  Bit D;
  Bit Edge;
} // namespace Bit

namespace InitPieces {
  Pieces init3 = {10, 10, 0, 0};
  Pieces init4 = {15, 15, 0, 0};
  Pieces init5 = {21, 21, 1, 1};
  Pieces init6 = {30, 30, 1, 1};
  Pieces init7 = {40, 40, 1, 1};
  Pieces init8 = {50, 50, 2, 2};
  Pieces init_default = {100, 100, 2, 2};
} // namespace InitPieces

namespace Weights {
  // positions       0 1 2 3   4   5   6   7   8    9
  eval_t GROUPS[] = {0,0,0,30,100,300,500,750,1100,1500};
  eval_t WIN = 2000000;
  eval_t CHECKWIN = 1000000;
  eval_t INF = 10000000;
  eval_t FLAT = 400;
  eval_t WALL = 200;
  eval_t CAP = 300;
  eval_t HARD_FCAPTIVE = 200;
  eval_t SOFT_FCAPTIVE = -200;
  eval_t HARD_SCAPTIVE = 300;
  eval_t SOFT_SCAPTIVE = -150;
  eval_t HARD_CCAPTIVE = 250;
  eval_t SOFT_CCAPTIVE = -150;
  eval_t CENTER = 7;
  eval_t ENDGAMEFLAT = 1400;
  float ENDGAMECUTOFF = 7;
} // namespace Weights

namespace HashVal {
  // 2**13 -> 12 stack places
  size_t num_entry = 8192;
  vector<size_t> stack_hash;
  vector<size_t> pos_hash;
} // namespace HashVal

void init_weights(){
  Weights::GROUPS[size] = Weights::WIN;
}

void init_pieces(){
  switch(size){
    case 5: default_pieces = InitPieces::init5; return;
    case 6: default_pieces = InitPieces::init6; return;
    case 7: default_pieces = InitPieces::init7; return;
    default: default_pieces = InitPieces::init_default; return;
  }
}

void init_bits(){
  Bits::Mask = (Bit(1) << size*size) - 1;
  for(int i=0; i<size; ++i){
    Bits::L |= Bit(1) << (i*size);
    Bits::R |= Bit(1) << (i*size + size - 1);
    Bits::U |= Bit(1) << (size*(size-1) + i);
    Bits::D |= Bit(1) << i;
  }
  Bits::Edge = Bits::L | Bits::R | Bits::U | Bits::D;
}

void init_slide(){
  // A[num_drops][num_pieces]->[vector of bits]
  cap_slides = vector<vector<vector<Bit>>>(size+1, vector<vector<Bit>>(size+1));
  all_slides = vector<vector<vector<Bit>>>(size+1, vector<vector<Bit>>(size+1));
  
  // Init A[0,0] = <0>
  cap_slides[0][0] = vector<Bit>(1, 0);

  // First generate A[i,j] for exactly i drops of j pieces
  for(int i=1; i<size+1; ++i){
    for(int j=i; j<size+1; ++j){
      auto &curr_slide = cap_slides[i][j];
      // A[i,j] can be got by append j-k to all elements of A[i-1,k]
      // A[i,j] = j-k || A[i-1,k]
      // i-1 <= k <= j-1
      for(int k=i-1; k<j; ++k){
        Bit l = (j-k) << (4*i);
        for(auto &x : cap_slides[i-1][k])
          curr_slide.push_back((l|x) + 1);
      }
    }
  }

  // Fill all_slides with atleast i drops and atleast j pieces
  // Fill cap_slides with exact i drops, atleast j pieces, last drop=1
  for(int i=size; i>0; --i){
    for(int j=size; j>0; --j){
      auto &curr_slide = all_slides[i][j];
      for(int ii=1; ii<=i; ++ii){
        for(int jj=1; jj<=j; ++jj){
          auto &v = cap_slides[ii][jj];
          curr_slide.insert(curr_slide.end(), v.begin(), v.end());
        }
      }
      vector<Bit> new_slide;
      for(int jj=1; jj<=j; ++jj){
        for(auto x : cap_slides[i][jj]){
          if((x >> (4*i)) == 1) new_slide.push_back(x);
        }
      }
      cap_slides[i][j] = new_slide;
    }
  }
}

void init_hash() {
  default_random_engine gen;
  uniform_int_distribution<size_t> r;
  HashVal::stack_hash = vector<size_t>(HashVal::num_entry);
  HashVal::pos_hash = vector<size_t>(size*size);
  for(auto &h : HashVal::stack_hash) h = r(gen);
  for(auto &h : HashVal::pos_hash) h = r(gen);
}

void init(int board_size) {
  size = board_size;
  init_slide();
  init_bits();
  init_pieces();
  init_weights();
  init_hash();
}

} // namespace Tak