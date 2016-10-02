#include "Header.h"

int Size = 3; 
int TimeLimit =1;
bool opponent_type = White;
const int max_depth = 6;

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
	cerr << "Enter player_no, size, TimeLimit\n";
	cin >> player_no >> Size >> TimeLimit;
	cerr << player_no << Size << TimeLimit;
	// int player_no;
	// cin >> player_no >> Size >> TimeLimit;
	Game g(Size);
	opponent_type = (player_no == 2) ? White : Black;
	getAllPerms(Size);

	if (!opponent_type == White)
		cerr << "You are White"  << endl;
	if (player_no == 1)
	{
		// i give oppo move:
		string s1 = "Fa1";
		cout << s1 << endl;
		g.make_opponent_move(s1,opponent_type);
		// now i get my piece as input.
		string mypiece_opp;
		cin >> mypiece_opp;
		g.make_opponent_move(mypiece_opp,!opponent_type);

		while (true)
		{
			Eval_Move mymove;
			g.decide_move(mymove, !opponent_type, 0, max_depth, -2*g.w[0], 2*g.w[0]);
			cerr << g.to_string() << endl;
			g.makemove(mymove.m);
			fprintf(stderr, "%s e = %f\n",mymove.m.to_string().c_str(), mymove.e);
			// cerr << (mymove.m.to_string()) << endl;
			cout << (mymove.m.to_string()) << endl;
			cerr << g.to_string() << endl;

			string s_opp;
			cin >> s_opp;
			cerr << "out received MOVE : " << s_opp << endl;
			g.make_opponent_move(s_opp,opponent_type);
			Test::print_index();
		}
	}
	else
	{
		string s;
		cin >> s;
		cerr << "out received MOVE : " << s << endl;
		g.make_opponent_move(s, !opponent_type);
		string s_opp;
		if (g.GameBoard[0][0].empty())
			s_opp = "Fa" + to_string(Size);
		else
			s_opp = "Fa1";

		cout << s_opp << endl;
		g.make_opponent_move(s_opp,opponent_type);

		while (true)
		{
			string s;
			cin >> s;
			cerr << "out received MOVE : " << s << endl;
			g.make_opponent_move(s, opponent_type);
			cerr << g.to_string() << endl;
			Eval_Move mymove;
			g.decide_move(mymove, !opponent_type, 0, max_depth, -2*g.w[0], 2*g.w[0]);
			cerr << g.to_string() << endl;
			g.makemove(mymove.m);
			fprintf(stderr, "%s e = %f\n",mymove.m.to_string().c_str(), mymove.e);
			// cerr << (mymove.m.to_string()) << endl;
			cout << (mymove.m.to_string()) << endl;
			cerr << g.to_string() << endl;
			Test::print_index();
		}
	}

	return 0;
}