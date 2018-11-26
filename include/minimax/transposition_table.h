#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include <vector>
#include <unordered_map>
#include "minimax/transposition.h"
#include "tak/bitboard.h"

#define TRANSPOSITION_MAX_SIZE 20000000

namespace Minimax {

using namespace Tak;

class TranspositionTable {
 public:
  Transposition &operator[](const BitBoard &b) {return ttable[b];}
  size_t size() {return ttable.size();}
 private:
  unordered_map<BitBoard, Transposition> ttable;
};

} // namespace Minimax

#endif