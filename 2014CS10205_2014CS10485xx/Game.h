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
	// void search(bool type, s_int x, s_int y, bool player, vector< vector<bool> > &explored, bool &found);
	void newsearch(s_int x, s_int y, bool player, vector< vector<bool> > &explored, vector<int> &lrud);

	// eval functions
	// eval_type path();
	eval_type newpath(bool);
	eval_type center(int,int);
	eval_type captive(char,pair<s_int,s_int>&);
	eval_type piece_type(char);

	void UpdatePlayer(Player_Type, Move&, bool);
	void GetStackable(s_int, s_int, bool, vector<s_int> &);
public:
	s_int size;
	vector< vector<Position> > GameBoard;
	Player p_black, p_white;
	// eval_type w[18];

	Game(s_int, s_int);
	eval_type eval(Player_Type);
	string to_string();
	void make_opponent_move(string, bool);
	void makemove(Move &);
	void antimove(Move &);
	eval_type features();
	Transposition& getTransposition(Player_Type);
	
	string ids();
	// int decide_Depth();
	bool isMoveValid(Move&,bool);
	void generate_place_1(Player_Type, list<Move>&);
	void generate_place_2(Player_Type, list<Move>&);
	void generate_stack_moves(Player_Type, list<Move>&);

	void generate_valid_moves(Player_Type, multimap<eval_type,Move>&);

	pair<eval_type, bool> negaMax(bool,s_int,eval_type,eval_type, pair<Move, Move> &);

	// temp
	void print_move_seq(int depth);
	void update_trans(Player_Type player, Transposition &t, int depth, eval_type best_val, Move *best_move, eval_type alpha_orig, eval_type beta, bool broken);
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
	return t;
}

inline eval_type Game::eval(Player_Type player){
	eval_type value = 0;
	value += newpath(player);
	value += features();
	return ((player == White) ? value+80 : -value+80);
}

inline void Game::update_trans(Player_Type player, Transposition &t, int depth, eval_type best_val, Move *best_move, eval_type alpha_orig, eval_type beta, bool broken){
	t.score = best_val;
	if (best_val <= alpha_orig)	t.flag = 'u';
	else if (best_val >= beta) t.flag = 'l';
	else t.flag = 'e';
	t.depth = broken? depth-1 : depth;
	t.best_move = *best_move;
}

#endif
