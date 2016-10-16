#ifndef GAME_H
#define GAME_H

#include "Position.hpp"
#include "Transposition.h"
using namespace Types;

class Game{
private:
	unordered_map<string, Transposition> TTable;
	
	// helper functions
	bool pathable(char x, char y, bool player);
	void search(bool type, s_int x, s_int y, bool player, vector< vector<bool> > &explored, bool &found)

	// eval functions
	eval_type path();

	void UpdatePlayer(Player_Type, Move&, bool);
	void Game::GetStackable(s_int, s_int, bool, vector<s_int> &);
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
};

inline bool pathable(s_int x, s_int y, bool player){
	return (!GameBoard[x][y].empty() && GameBoard[x][y].top_piece() != 'S' && GameBoard[x][y].player() == player);
}

#endif