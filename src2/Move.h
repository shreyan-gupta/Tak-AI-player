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