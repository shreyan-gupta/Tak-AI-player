#ifndef GAME_H
#define GAME_H

#include "Position.h"
#include "Transposition.h"
using namespace Types;

class Game{
private:
	vector<unordered_map<string, Transposition> > TTable; // length 2.
	
	// helper functions
	bool pathable(char x, char y, bool player);
	void search(bool type, s_int x, s_int y, bool player, vector< vector<bool> > &explored, bool &found);

	// eval functions
	eval_type path();

	void UpdatePlayer(Player_Type, Move&, bool);
	void GetStackable(s_int, s_int, bool, vector<s_int> &);
	void getTransposition(Transposition &,Player_Type);
	eval_type eval();
public:
	s_int size;
	vector< vector<Position> > GameBoard;
	Player p_black, p_white;
	eval_type w[18];

	Game(s_int, s_int);
	string to_string();
	void make_opponent_move(string, bool);
	void makemove(Move &);
	void antimove(Move &);
	
	void generate_place_1(Player_Type, list<Move>&);
	void generate_place_2(Player_Type, list<Move>&);
	void generate_stack_moves(Player_Type, list<Move>&);

	eval_type negaMax(bool,char,eval_type,eval_type);
	// CALL decide_move after negaMax?
};

inline bool Game::pathable(s_int x, s_int y, bool player){
	return (!GameBoard[x][y].empty() && GameBoard[x][y].top_piece() != 'S' && GameBoard[x][y].player() == player);
}

inline void Game::getTransposition(Transposition &t, Player_Type p){
	t = TTable[p][to_string()];
	if(t.flag == 'x'){
		t.flag = 'e';
		t.score = eval();
		t.depth = 0;
	}
}

inline eval_type eval(){
	eval_type value = 0;
	value += path();
	return value;
}

#endif