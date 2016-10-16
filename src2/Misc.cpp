#include "Move.h"
using namespace Types;

vector< vector< vector< vector<s_int> > > > AllPerms;

Player::Player(bool player_type, s_int pieces){
	type = player_type;
	StonesLeft = pieces;
	CapLeft = true;
	x = -1;
	y = -1;
}

void getAllPerms(s_int dim)
{
	// fill this matrix!
	// AllPerms[i][j] = all vectors with size = j, sum = i.
	AllPerms = vector<vector<vector<vector<s_int> > > > (dim+1, std::vector<vector<vector<s_int> > > (dim));
	for (s_int sum = 1 ; sum <= dim ; sum ++)
	{
		for (s_int size = 1 ; size < dim ; size ++)
		{
			vector<vector<s_int> > & fillthis = AllPerms[sum][size];
			if (size == 1)
			{
				vector<s_int> x (1,sum);
				fillthis.push_back(x);
			}
			s_int i = sum-1;
			while (i > 0)
			{
				// pick all vectors at [i][size-1]
				for (auto &j : AllPerms[i][size-1])
				{
					vector<s_int> x = j;
					x.push_back(sum-i);
					fillthis.push_back(x);
				}
				i --;
			}
		}
	}
}

string Move::to_string()
{
	string s = "";
	s += (char)('a' + y);
	s += (char)('0' + size - x);
	if (place_move)
	{
		// append F or S or C.
		switch (piece)
		{
			case 'f':
				s = "F" + s;
				break;
			case 's':
				s = "S" + s;
				break;
			case 'c':
				s = "C" + s;
				break;
			default:
				s = piece + s;
				break;
		}
	}
	else
	{
		// move stack!
		int sum = 0;
		switch (direction)
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
		for (auto &i : *drops)
		{
			s += Types::to_string((int)i);
			sum += i;
		}
		return (Types::to_string(sum) + s);
	}
	return s;
}

Move::Move(s_int x, s_int y, char p) : drops(NULL)
{
	this->cap_move = false;
	this->place_move = true;
	this->x = x;
	this->y = y;
	this->piece = p;
}

Move::Move(s_int x, s_int y, char d, vector<s_int> *v){
	this->cap_move = false;
	this->place_move = false;
	this->x = x;
	this->y = y;
	this->direction = d;
	this->drops = v;
}

Move::Move() {}