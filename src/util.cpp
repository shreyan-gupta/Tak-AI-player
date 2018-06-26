#include "util.h"

namespace Tak {

using namespace SlideVec;

// Global variables
int size;
vector<vector<vector<Bit>>> cap_slides;
vector<vector<vector<Bit>>> all_slides;

void init_bits(){
  Bits::Mask = (1 << size*size) - 1;
  for(int i=0; i<size; ++i){
    Bits::L |= 1 << (i*size);
    Bits::R |= 1 << (i*size + size - 1);
    Bits::U |= 1 << (size*(size-1) + i);
    Bits::D |= 1 << i;
  }
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

void init(int board_size) {
  size = board_size;
  init_bits();
  init_slide();
}

} // namespace Tak