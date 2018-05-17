#ifndef MOVE_H
#define MOVE_H

#include "Header.h"
using namespace Types;

struct Move
{
  bool cap_move = false;
  bool place_move;  // true if place else false
  s_int x;
  s_int y;
  char piece;
  char direction;   // + U, - D, > R, < L
  vector<s_int> *drops;
  
  Move(s_int, s_int, char);
  Move(s_int, s_int, char, vector<s_int> *);
  Move();
  string to_string(); // print ke liye!
  Player_Type player();
};

inline Player_Type Move::player(){
  return (piece < 95);
}

#endif