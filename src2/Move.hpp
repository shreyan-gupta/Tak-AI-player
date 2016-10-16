#include "Header.h"
using namespace Types;

struct Move
{
	bool cap_move = false;
	bool place_move; 	// true if place else false
	s_int x;
	s_int y;
	char piece;
	char direction; 	// + U, - D, > R, < L
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

string Move::to_string()
{
	string s = "";
	s += (char)('a' + y);
	s += (char)('0' + Size - x);
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
		for (auto &i : *Drops)
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