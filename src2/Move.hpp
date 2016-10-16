#include "Header.h"

struct Move
{
	bool cap_move = false;
	bool place_move; 	// true if place else false
	char x;
	char y;
	char piece; 			// rakhna hai ye/uthaana hai ye.
	char direction; 	// + U, - D, > R, < L
	vector<char> *drops;
	string to_string(); // print ke liye!
	Move(char, char, char);
	Move(char, char, char, vector<char> *);
	Move();
};

string Move::to_string()
{
	string s = "";
	s += (char)('a' + y);
	s += (char)('0' + Size - x);
	if (Place_Move)
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
			s += Types::to_string((int)i);
			sum += i;
		}
		return (Types::to_string(sum) + s);
	}
	return s;
}

Move::Move(char x, char y, char p) : drops(NULL)
{
	this->cap_move = false;
	this->place_move = true;
	this->x = x;
	this->y = y;
	this->piece = p;
}

Move::Move(char x, char y, char d, vector<char> *v){
	this->cap_move = false;
	this->place_move = false;
	this->x = x;
	this->y = y;
	this->direction = d;
	this->drops = v;
}

Move::Move() {}