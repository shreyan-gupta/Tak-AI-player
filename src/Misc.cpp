#include "Header.h"
using namespace Types;

Player::Player(bool player_type, int pieces, int caps){
	type = player_type;
	StonesLeft = pieces;
	CapsLeft = caps;
}

Move::Move(int x, int y, Piece p) : Drops(NULL)
{
	this->Place_Move = true;
	this->x = x;
	this->y = y;
	this->p = p;
}

Move::Move(int x, int y, char d, vector<int> *v){
	this->Place_Move = false;
	this->x = x;
	this->y = y;
	this->Direction = d;
	this->Drops = v;
}

bool Eval_Move::operator<(const Eval_Move &other_move){
	return (this->e < other_move.e);
}

bool Eval_Move::operator>(const Eval_Move &other_move){
	return (this->e > other_move.e);
}

Eval_Move::Eval_Move(eval_type &e, Move &m) : e(e), m(m) {}
Eval_Move::Eval_Move(const Eval_Move &other_move) : e(other_move.e), m(other_move.m) {}

string Position::to_string(){
	int val=0;
	int mul = 1;
	string s = "";
	for(auto &i : Stack){
		if(i.second == Black) val += mul;
		mul = mul << 1;
	}
	val += mul;
	s += std::to_string(val);
	if(!empty()){
		if(Stack.back().first == Flat) s += "F";
		else if(Stack.back().first == Stand) s+= "S";
		else s += "C";
	}
	return s;
}