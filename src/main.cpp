#include "Header.h"

int Size, TimeLimit;
bool opponent_type;

void printVec(vector<int> & v){
	for (auto &i : v) std::cout << i;
	std::cout << ",";
}

int main(int argc, char const *argv[])
{
	// SET opponent_type !!!!
	int player_no;
	cin >> player_no >> Size >> TimeLimit;
	Game g(Size);
	opponent_type = (player_no == 2) ? White : Black;
	getAllPerms(Size);

	if (!opponent_type == White)
		cout << "I am White"  << endl;
	while (true)
	{
		string s;
		cin >> s;
		g.make_opponent_move(s);
		cout << g.to_string() << endl;
		Eval_Move mymove;
		g.decide_move(mymove,!opponent_type,0,4);
		cout << (mymove.m.to_string()) << endl;
		g.makemove(mymove.m);
	}


	bool valid = Test::checkValid();
	if (valid) cout << "valid moves check karo \n";

	return 0;
}