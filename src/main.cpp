#include "Header.h"

int Size = 3; 
int TimeLimit =1;
bool opponent_type = White;

void printVec(vector<int> & v){
	for (auto &i : v) std::cout << i;
	std::cout << ",";
}

int main(int argc, char const *argv[])
{
	// SET opponent_type !!!!
	// int player_no;
	// cin >> player_no >> Size >> TimeLimit;
	Game g(Size);
	// opponent_type = (player_no == 2) ? White : Black;
	getAllPerms(Size);

	if (!opponent_type == White)
		cout << "You are White"  << endl;
	while (true)
	{
		string s;
		cin >> s;
		g.make_opponent_move(s);
		Eval_Move mymove;
		g.decide_move(mymove,!opponent_type,0,2);
		cout << g.to_string() << endl;
		g.makemove(mymove.m);
		cout << (mymove.m.to_string()) << endl;
	}


	bool valid = Test::checkValid();
	if (valid) cout << "valid moves check karo \n";

	return 0;
}