#include "Header.h"

int Size = 3; 
int TimeLimit =1;
bool opponent_type = White;

void printVec(vector<int> & v){
	for (auto &i : v) std::cerr << i;
	std::cerr << ",";
}

int main(int argc, char const *argv[])
{
	// SET opponent_type !!!!
	// Size = 3;
	// getAllPerms(3);
	// bool valid = Test::checkfavourable();
	// if (valid) cerr << "valid moves check karo \n";

	int player_no = 2;
	TimeLimit = 1000;
	Size = 3;
	// cin >> player_no >> Size >> TimeLimit;
	// int player_no;
	// cin >> player_no >> Size >> TimeLimit;
	Game g(Size);
	// opponent_type = (player_no == 2) ? White : Black;
	getAllPerms(Size);

	if (!opponent_type == White)
		cerr << "You are White"  << endl;
	while (true)
	{
		string s;
		cin >> s;
		g.make_opponent_move(s);
		Eval_Move mymove;
		g.decide_move(mymove,!opponent_type,0,2);
		cerr << g.to_string() << endl;
		g.makemove(mymove.m);
		cerr << (mymove.m.to_string()) << endl;
		cerr << g.to_string() << endl;
	}



	return 0;
}