#include "Game.h"
#include "Move.h"

s_int size, pieces;
int TimeLimit;
bool opponent_type;
time_t start_time;
vector<int> GroupWt;

void checkMove()
{
  size = 3;
  Game g(size,21);
  eval_type boardeval;
  Move m1(1,1,'F');
  g.makemove(m1);
  boardeval = g.eval(White);
  printf("%s is Board after m1 \n", g.to_string().c_str());

  Move m2(0,2,'F');
  g.makemove(m2);
  boardeval = g.eval(White);
  printf("%s is Board after m2 \n", g.to_string().c_str());

  Move m3(0,1,'C');
  g.makemove(m3);
  boardeval = g.eval(White);
  printf("%s is Board after m3 \n", g.to_string().c_str());

  Move m5(0,0,'F');
  g.makemove(m5);
  boardeval = g.eval(White);
  printf("%s is Board after m5 \n", g.to_string().c_str());


  vector<s_int> v (1,1);
  Move m4(0,1,'+', &v);
  m4.cap_move = true;
  g.makemove(m4);
  boardeval = g.eval(White);
  printf("%s is Board after m4 \n", g.to_string().c_str());

  g.antimove(m4);
  printf("%s is Board after m3 \n", g.to_string().c_str());

  g.antimove(m3);
  printf("%s is Board after m2 \n", g.to_string().c_str());

  g.antimove(m2);
  printf("%s is Board after m1 \n", g.to_string().c_str());
}

void check_makeOppo()
{
  size = 3;
  Game g(size,21);
  Move m1(2,1,'F');
  cout << "Yo, in move \n";
  cout << "Move is " << m1.to_string() << endl;
  g.make_opponent_move(m1.to_string(), White);
  printf("%s is Board after m1 \n", g.to_string().c_str());

  Move m2(1,1,'f');
  cout << "m2 is " << m2.to_string() << endl;
  g.make_opponent_move(m2.to_string(), Black);
  printf("%s is Board after m2 \n", g.to_string().c_str());

  Move m3(0,1,'C');
  g.make_opponent_move(m3.to_string(), White);
  printf("%s is Board after m3 \n", g.to_string().c_str());

  vector<s_int> v (1,1);
  Move m4(0,1,'+', &v);
  // m4.cap_move = true;
  g.make_opponent_move(m4.to_string(), White);
  printf("%s is Board after m4 \n", g.to_string().c_str());

  // g.antimove(m4);
  // printf("%s is Board after m3 \n", g.to_string().c_str());

  // g.antimove(m3);
  // printf("%s is Board after m2 \n", g.to_string().c_str());

  // g.antimove(m2);
  // printf("%s is Board after m1 \n", g.to_string().c_str());
}

void printMoveList(list<Move> &m)
{
  for (auto it = m.begin(); it != m.end(); it++)
    cout << (*it).to_string() << endl;
}

// void checkValid1()
// {
//  size = 3;
//  Game g(size,21);
//  cout << "All possible moves for White before m1 \n";
//  list<Move> l1;
//  g.generate_place_1(White, l1);
//  printMoveList(l1);

//  Move m1(2,1,'F');
//  g.makemove(m1);
//  printf("%s is Board after m1 \n", g.to_string().c_str());

//  cout << "All possible moves for Black before m2 \n";
//  list<Move> l2;
//  g.generate_place_1(Black, l2);
//  printMoveList(l2);

//  Move m2(1,1,'s');
//  g.makemove(m2);
//  printf("%s is Board after m2 \n", g.to_string().c_str());

//  cout << "All possible moves for White before m2 \n";
//  list<Move> l3;
//  g.generate_place_1(White, l3);
//  printMoveList(l3);
//  Move m3(0,1,'C');
//  g.makemove(m3);
//  printf("%s is Board after m3 \n", g.to_string().c_str());
// }

// void checkValid2()
// {
//  size = 3;
//  Game g(size,21);
//  cout << "All possible moves for White before m1 \n";
//  list<Move> l1;
//  g.generate_place_2(White, l1);
//  printMoveList(l1);

//  Move m1(2,1,'F');
//  g.makemove(m1);
//  printf("%s is Board after m1 \n", g.to_string().c_str());

//  cout << "All possible moves for Black before m2 \n";
//  list<Move> l2;
//  g.generate_place_2(Black, l2);
//  printMoveList(l2);

//  Move m2(1,1,'s');
//  g.makemove(m2);
//  printf("%s is Board after m2 \n", g.to_string().c_str());

//  cout << "All possible moves for White before m3 \n";
//  list<Move> l3;
//  g.generate_place_2(White, l3);
//  printMoveList(l3);
//  Move m3(0,1,'C');
//  g.makemove(m3);
//  printf("%s is Board after m3 \n", g.to_string().c_str());
// }

// void checkStackMoves()
// {
//  size = 3;
//  Game g(3,21);
//  cout << "All possible moves for White before m1 \n";
//  list<Move> l1;
//  g.generate_stack_moves(White, l1);
//  printMoveList(l1);

//  Move m1(2,1,'F');
//  g.makemove(m1);
//  printf("%s is Board after m1 \n", g.to_string().c_str());

//  cout << "All possible moves for Black before m2 \n";
//  list<Move> l2;
//  g.generate_stack_moves(Black, l2);
//  printMoveList(l2);

//  Move m2(1,1,'s');
//  g.makemove(m2);
//  printf("%s is Board after m2 \n", g.to_string().c_str());

//  cout << "All possible moves for White before m3 \n";
//  list<Move> l3;
//  g.generate_stack_moves(White, l3);
//  printMoveList(l3);

//  Move m3(0,1,'C');
//  g.makemove(m3);
//  printf("%s is Board after m3 \n", g.to_string().c_str());

//  cout << "All possible moves for White before m4 \n";
//  list<Move> l4;
//  g.generate_stack_moves(White, l4);
//  printMoveList(l4);

//  vector<s_int> v (1,1);
//  Move m4(0,1,'+', &v);
//  m4.cap_move = true;
//  g.makemove(m4);
//  printf("%s is Board after m4 \n", g.to_string().c_str());

//  cout << "All possible moves for White after m4 \n";
//  list<Move> l5;
//  g.generate_stack_moves(White, l5);
//  printMoveList(l5);
// }

int main(int argc, char const *argv[])
{
  cout << "Yo \n";
  // getAllPerms(3);
  checkMove();
  // check_makeOppo();
  // checkValid1();
  // checkValid2();
  // checkStackMoves();
  return 0;
}