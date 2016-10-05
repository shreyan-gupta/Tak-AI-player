#include "Header.h"

int Size = 3; 
int TimeLimit =1;
bool opponent_type = White;
time_t start_time;

int prune_count[10] = {0,0,0,0,0,0,0,0,0,0};
int prune_index[10] = {0,0,0,0,0,0,0,0,0,0};
int depth_count[10] = {0,0,0,0,0,0,0,0,0,0};
time_t time_taken[10] = {0,0,0,0,0,0,0,0,0,0};
time_t total_time = 0;
int moves = 0;
int max_depth = 6;

// void printVec(vector<int> & v){
// 	for (auto &i : v) std::cerr << i;
// 	std::cerr << ",";
// }

// void print_avg_time(){
// 	FILE *debug;
// 	debug = fopen("debug.txt", "a");
// 	fprintf(debug, "Depth count ");
// 	for(int i=3; i<=max_depth; ++i){
// 		fprintf(debug, "%d : %d\t", i, depth_count[i]);
// 	}
// 	fprintf(debug, "Avg time ");
// 	for(int i=3; i<max_depth; ++i){
// 		if(depth_count[i] != 0)
// 			fprintf(debug, "%f\t", (1.0*total_time/depth_count[i]));
// 		else 
// 			fprintf(debug, "_______\t");
// 	}
// 	fprintf(debug, "\n");
// 	fclose(debug);
// }

// void test_cap(Game &g){
// 	if(g.p_black.x != -1 && g.p_black.y != -1){
// 		if(g.GameBoard[g.p_black.x][g.p_black.y].top_piece().first != Cap){
// 			string s;
// 			if(g.GameBoard[g.p_black.x][g.p_black.y].top_piece().first == Cap) s = "Cap";
// 			else if(g.GameBoard[g.p_black.x][g.p_black.y].top_piece().first == Flat) s = "Flat";
// 			else s = "Stand";
// 			fprintf(stderr, "Black Cap ki coordinates are wrong!!! %d %d %s\n",g.p_black.x, g.p_black.y, s.c_str());
// 		}
// 	}

// 	if(g.p_white.x != -1 && g.p_white.y != -1){
// 		if(g.GameBoard[g.p_white.x][g.p_white.y].top_piece().first != Cap){
// 			string s;
// 			if(g.GameBoard[g.p_white.x][g.p_white.y].top_piece().first == Cap) s = "Cap";
// 			else if(g.GameBoard[g.p_white.x][g.p_white.y].top_piece().first == Flat) s = "Flat";
// 			else s = "Stand";
// 			fprintf(stderr, "White Cap ki coordinates are wrong!!! %d %d %s\n",g.p_white.x, g.p_white.y, s.c_str());
// 		}
// 	}
// }

int manage_depth(Game &g){
	if(moves < Size/2 + 1) return 5;
	if(moves < Size/2 + 1 + 3) return 6;

	int empty_squares = 0;
	for(auto &i : g.GameBoard)
		for(auto &j : i)
			if(j.empty()) ++empty_squares;

	float avg_time = (float)(1.2)*(TimeLimit - total_time - 2)/(10 + empty_squares);

	if(avg_time > (float)time_taken[max_depth]/((float)depth_count[max_depth] - 0.1)){
		max_depth = min(max_depth+1, 6);
		fprintf(stderr, "!Depth : %d t_left %d : %f %f\n", max_depth, (int)(TimeLimit - total_time), avg_time, (float)time_taken[max_depth]/((float)depth_count[max_depth] - 0.1));
		return min(max_depth,6);
	}
	else{
		int temp = max_depth;
		while(temp >= 2 &&  avg_time < (float)time_taken[temp]/((float)depth_count[temp] - 0.1)) --temp;
		fprintf(stderr, "Depth : %d t_left %d : %f %f\n", temp, (int)(TimeLimit - total_time), avg_time, (float)time_taken[temp]/((float)depth_count[temp] - 0.1));
		return temp;
	}
	// time left/(10 + 2*empty) itni aim for a depth.
	// stop when time since this search*abhi branch factor > 2*aim...
}

int main(int argc, char const *argv[])
{
	start_time = time_t(0);

	int player_no;
		cerr << "Enter player_no, size, TimeLimit\n";
	cin >> player_no >> Size >> TimeLimit;

	char pieces;
	switch(Size){
		case 5 : pieces = 21; break;
		case 6 : pieces = 30; break;
		case 7 : pieces = 40; break;
		default : pieces = 10;
	}

	Game g(Size, pieces);
	opponent_type = (player_no == 2) ? White : Black;
	getAllPerms(Size);

		// if (!opponent_type == White) cerr << "You are White"  << endl;
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
		++moves;

		while (true)
		{
			Eval_Move mymove;
			time_t start_move_time = time(0);
			int depth = manage_depth(g);
			g.decide_move(mymove, !opponent_type, 0, depth, -2*g.w[0], 2*g.w[0]);
			time_t time_move = (time(0) - start_move_time);
			total_time += time_move;
			for (int i = 0; i <= depth; i++)
				time_taken[i] += time_move;
				// print_avg_time();
				// cerr << "Time " << total_time << endl;
				// cerr << g.to_string() << endl;
			g.makemove(mymove.m);
				fprintf(stderr, "%s e = %f\n",mymove.m.to_string().c_str(), mymove.e);
				// test_cap(g);

			cout << (mymove.m.to_string()) << endl;
				// cerr << g.to_string() << endl;
			string s_opp;
			cin >> s_opp;
				// cerr << "out received MOVE : " << s_opp << endl;
			g.make_opponent_move(s_opp,opponent_type);
				// Test::print_index();
			++moves;
		}
	}
	else
	{
		string s;
		cin >> s;
			// cerr << "out received MOVE : " << s << endl;
		g.make_opponent_move(s, !opponent_type);
		string s_opp;
		if (g.GameBoard[0][0].empty())
			s_opp = "Fa" + to_string(Size);
		else
			s_opp = "Fa1";
		++moves;
		cout << s_opp << endl;
		g.make_opponent_move(s_opp,opponent_type);
		
		while (true)
		{
			string s;
			cin >> s;
				// cerr << "out received MOVE : " << s << endl;
			g.make_opponent_move(s, opponent_type);
				// cerr << g.to_string() << endl;
			Eval_Move mymove;
			time_t start_move_time = time(0);
			int depth = manage_depth(g);
			g.decide_move(mymove, !opponent_type, 0, depth, -2*g.w[0], 2*g.w[0]);
			time_t time_move = (time(0) - start_move_time);
			total_time += time_move;
			for (int i = 0; i <= depth; i++)
				time_taken[i] += time_move;
				// print_avg_time();
				// cerr << "Time " << total_time << endl;
				// cerr << g.to_string() << endl;
			g.makemove(mymove.m);
				fprintf(stderr, "%s e = %f\n",mymove.m.to_string().c_str(), mymove.e);
				// fprintf(stderr, "White CAP %d %d\n",g.p_white.x, g.p_white.y);
				// fprintf(stderr, "Black CAP %d %d\n",g.p_black.x, g.p_black.y);
				// test_cap(g);

			cout << (mymove.m.to_string()) << endl;
				// cerr << g.to_string() << endl;
				// Test::print_index();
			++moves;
		}
	}

	return 0;
}