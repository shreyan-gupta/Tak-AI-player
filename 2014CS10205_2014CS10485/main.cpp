#include "Game.h"

s_int size, pieces;
int TimeLimit;
bool opponent_type;
TPoint start_time;


int main(int argc, char const *argv[])
{
	int temp_size;
	start_time = Time::now();
	int player_no;
		cerr << "Enter player_no, size, TimeLimit\n";
	cin >> player_no >> temp_size >> TimeLimit;
	size = temp_size;
	--TimeLimit;
	// char pieces;
	switch(size){
		case 5 : pieces = 21; break;
		case 6 : pieces = 30; break;
		case 7 : pieces = 40; break;
		default : pieces = 10;
	}

	Game g(size, pieces);
	opponent_type = (player_no == 2) ? White : Black;
	getAllPerms(size);

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
			string my_move = g.ids();
			cout << my_move << endl;

			string s_opp;
			cin >> s_opp;
			g.make_opponent_move(s_opp,opponent_type);

		}
	}


	else
	{
		string s;
		cin >> s;
		g.make_opponent_move(s, !opponent_type);
		string s_opp;
		if (g.GameBoard[0][0].empty())
			s_opp = "Fa" + to_string(size);
		else
			s_opp = "Fa1";
		cout << s_opp << endl;
		g.make_opponent_move(s_opp,opponent_type);
		
		while (true)
		{
			string s;
			cin >> s;
			g.make_opponent_move(s,opponent_type);
			
			string my_move = g.ids();
			cout << my_move << endl;	
		}
	}

	return 0;
}