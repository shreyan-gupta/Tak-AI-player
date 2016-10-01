#include "Header.h"
using namespace Types;

Player::Player(bool player_type, int pieces){
	type = player_type;
	StonesLeft = pieces;
	CapLeft = true;
	x = -1;
	y = -1;
}

string Move::to_string()
{
	string s = "";
	s += (char)('a' + y);
	s += (char)('0' + Size - x);
	if (Place_Move)
	{
		switch (p.first)
		{
			case Flat:
				return "F" + s;
			case Stand:
				return "S" + s;
			case Cap:
				return "C" + s;
		}
	}
	else
	{
		// move stack!
		int sum = 0;
		switch (Direction)
		{
			case '<':
				s += '<';
				break;
			case '>':
				s += '>';
				break;
			case '+':
				s += '-';
				break;
			case '-':
				s += '+';
				break;
		}
		for (auto &i : *Drops)
		{
			s += Types::to_string(i);
			sum += i;
		}
		return (Types::to_string(sum) + s);
	}
	return s;
}

Move::Move(int x, int y, Piece p) : Drops(NULL)
{
	this->CapMove = false;
	this->Place_Move = true;
	this->x = x;
	this->y = y;
	this->p = p;
}

Move::Move(int x, int y, char d, vector<int> *v){
	this->CapMove = false;
	this->Place_Move = false;
	this->x = x;
	this->y = y;
	this->Direction = d;
	this->Drops = v;
}

Move::Move() {}

bool Eval_Move::operator<(const Eval_Move &other_move){
	return (this->e < other_move.e);
}

bool Eval_Move::operator>(const Eval_Move &other_move){
	return (this->e > other_move.e);
}

Eval_Move::Eval_Move(eval_type &e, Move &m) : e(e), m(m) {}
Eval_Move::Eval_Move(const Eval_Move &other_move) : e(other_move.e), m(other_move.m) {}
Eval_Move::Eval_Move() {}

string Position::to_string(){
	int val=0;
	int mul = 1;
	string s = "";
	for(auto i = Stack.rbegin() ; i != Stack.rend() ; i ++){
		if(i->second == White) val += mul;
		mul = mul << 1;
	}
	val += mul;
	s += Types::to_string(val);
	if(!empty()){
		if(Stack.front().first == Flat) s += "F";
		else if(Stack.front().first == Stand) s+= "S";
		else s += "C";
	}
	return s;
}


vector< vector< vector< vector<int> > > > AllPerms;

void getAllPerms(int dim)
{
	// fill this matrix!
	// AllPerms[i][j] = all vectors with size = j, sum = i.
	AllPerms = vector<vector<vector<vector<int> > > > (dim+1, std::vector<vector<vector<int> > > (dim));
	for (int sum = 1 ; sum <= dim ; sum ++)
	{
		for (int size = 1 ; size < dim ; size ++)
		{
			vector<vector<int> > & fillthis = AllPerms[sum][size];
			if (size == 1)
			{
				vector<int> x (1,sum);
				fillthis.push_back(x);
			}
			int i = sum-1;
			while (i > 0)
			{
				// pick all vectors at [i][size-1]
				for (auto &j : AllPerms[i][size-1])
				{
					vector<int> x = j;
					x.push_back(sum-i);
					fillthis.push_back(x);
				}
				i --;
			}
		}
	}
}