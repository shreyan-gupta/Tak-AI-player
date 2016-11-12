#ifndef GAME_H
#define GAME_H

#include "Position.h"
#include "Transposition.h"
using namespace Types;

class Game{
private:
	vector<unordered_map<string, Transposition> > TTable; // length 2.
	
	// helper functions
	bool pathable(s_int x, s_int y, bool player);
	void search(bool type, s_int x, s_int y, bool player, vector< vector<bool> > &explored, bool &found);

	// eval functions
	eval_type path();
	eval_type center(int,int);
	eval_type captive(char,pair<s_int,s_int>&);
	eval_type piece_type(char);

	void UpdatePlayer(Player_Type, Move&, bool);
	void GetStackable(s_int, s_int, bool, vector<s_int> &);
	eval_type eval(Player_Type);
public:
	s_int size;
	vector< vector<Position> > GameBoard;
	Player p_black, p_white;
	// eval_type w[18];

	Game(s_int, s_int);
	string to_string();
	void make_opponent_move(string, bool);
	void makemove(Move &);
	void antimove(Move &);
	eval_type features();
	Transposition& getTransposition(Player_Type);
	
	string ids(int);
	int decide_Depth();
	bool isMoveValid(Move&,bool);
	void generate_place_1(Player_Type, list<Move>&);
	void generate_place_2(Player_Type, list<Move>&);
	void generate_stack_moves(Player_Type, list<Move>&);

	void generate_valid_moves(Player_Type, multimap<pair<eval_type,eval_type>,Move>&);

	eval_type negaMax(bool,s_int,eval_type,eval_type, pair<Move, Move> &, bool);
	// CALL decide_move after negaMax?

	// temp
	void print_move_seq(int depth);
	void update_trans(Transposition &t, int depth, eval_type best_val, Move *best_move, eval_type alpha_orig, eval_type beta, bool is_null_window);
};

inline bool Game::pathable(s_int x, s_int y, bool player){
	return (!GameBoard[x][y].empty() && GameBoard[x][y].top_piece() != 'S' && GameBoard[x][y].player() == player);
}

inline Transposition& Game::getTransposition(Player_Type p){
	auto &t = TTable[p][to_string()];
	if(t.flag == 'x'){
		t.flag = 'e';
		t.score = eval(p);
		t.depth = 0;
	}
	// cout << "Depth of trans = " << t.depth << endl;
	return t;
}

inline eval_type Game::eval(Player_Type player){
	eval_type value = 0;
	value += path();
	if (abs(value) > FLWIN/2)
		return ((player == White) ? value : -value);
	value += features();
	return ((player == White) ? value : -value);
}

inline void Game::update_trans(Transposition &t, int depth, eval_type best_val, Move *best_move, eval_type alpha_orig, eval_type beta, bool is_null_window){
	if(is_null_window) return;
	// if(to_string().compare("F_F_f__F___f_____f___F_F_c___f_____") == 0){
	// 	fprintf(stderr, "UPDATING TRANS\n");
	// 	fprintf(stderr, "%s\n", t.to_string().c_str());
	// }
	t.score = best_val;
	if (best_val <= alpha_orig)	t.flag = 'u';
	else if (best_val >= beta) t.flag = 'l';
	else t.flag = 'e';
	t.depth = depth;
	t.best_move = *best_move;

	assert(!(t.best_move.x == -1 || t.depth <= 0));
	// if (t.best_move.x == -1 || t.depth <= 0)
	// {
	// 	cerr << t.best_move.x << ", depth = " << t.depth << endl;
	// 	cerr << "ERRORRRRRRR INVALID MOVE! \n";
	// 	int c;
	// 	cin >> c;
	// }
}

#endif
