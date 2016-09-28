#include "Header.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))

using namespace Types;

eval_type E_MIN = -99999999999;
eval_type E_MAX = +99999999999;
Piece NULL_PIECE = make_pair(Flat, false);
Move NULL_MOVE = Move(-1, -1, NULL_PIECE);

Game::Game(int size) : p_white(Player(White, 100)), p_black(Player(Black, 100))
{
	this->size = size;
	GameBoard = vector< vector<Position> >(size, vector<Position>(size));
	f[0] = &Game::feature0;
	f[1] = &Game::feature1;
	f[2] = &Game::feature2;
	f[3] = &Game::feature3;
	weight[0] = 1;
	weight[1] = 1;
	weight[2] = 1;
	weight[3] = 1;
}

string Game::to_string()
{
	string str = "";
	for (int i = 0 ; i < size ; i ++)
	{
		for (int j = 0 ; j < size ; j ++)
		{
			str += GameBoard[i][j].to_string() + "_"; // this is a position.
		}
	}
	return str;
}

eval_type Game::eval(){
	eval_type e = 0;
	for(int i=0; i< 4; ++i){
		e += CALL_MEMBER_FN(this, f[0]) ();
	}
	return e;
}

void Game::decide_move(Eval_Move &best_move, bool player, int depth, int max_depth){
	
	multimap <eval_type, Move> allmoves;
	generate_valid_moves(player, allmoves);
	
	if(depth == max_depth){
		auto ptr = allmoves.begin();
		best_move.e = ptr->first;
		best_move.m = ptr->second;
		return;
	}

	Eval_Move opponent_move(E_MIN, NULL_MOVE);
	//iterate over all valid moves
	for (auto &i : allmoves){
		// makemove.
		// ask !player to decide his best move
		// make anti move.
		
		makemove(i.second);
		decide_move(opponent_move, !player, depth+1, max_depth);
		if(player && opponent_move > best_move){
			best_move = opponent_move;
		}else if(!player && opponent_move < best_move){
			best_move = opponent_move;
		}
	}
}

void Game::makemove(Move &m)
{
	// TODO
	if (m.Place_Move){
		// x,y pe posn mein push kardo. (stack must be empty abhi.)
		// push WHAT?? 
		GameBoard[m.x][m.y].Stack.push_front(m.p);
		if (m.p.second == Black)
			GameBoard[m.x][m.y].Num_Black += 1;
		else
			GameBoard[m.x][m.y].Num_White += 1;
	}else{
		// move/anti?
		vector<int> &d = *m.Drops;
		char dirn = m.Direction;
		int y_add = (dirn == '>') ? 1 : ((dirn == '<') ? -1 : 0);
		int x_add = (dirn == '+') ? 1 : ((dirn == '-') ? -1 : 0);
		int drop_x = m.x + x_add;
		int drop_y = m.y + y_add;

		std::deque<Piece> & mainstack = GameBoard[m.x][m.y].Stack;

		for (int i = 0 ; i < d.size() ; i ++)
		{
			int num_drops = d[i];
			for (int j = num_drops - 1 ; j > -1 ; j --)
			{
				Piece jth = mainstack[mainstack.size() - 1 - j];
				// printf("%d %d %d %d %d Pushing piece to front \n", drop_x, drop_y, m.x, m.y, mainstack.size());
				if (jth.second == White)
					cout << "White pushed to front \n";
				GameBoard[drop_x][drop_y].Stack.push_front(jth);
				if (jth.second == Black)
					GameBoard[drop_x][drop_y].Num_Black += 1;
				else
					GameBoard[drop_x][drop_y].Num_White += 1;
			}
			// pop all these!
			for (int j = 0 ; j < num_drops ; j ++)
			{
				if (mainstack.front().second == Black)
					GameBoard[m.x][m.y].Num_Black -= 1;
				else
					GameBoard[m.x][m.y].Num_White -= 1;
				mainstack.pop_front();
			}
			drop_x += x_add;
			drop_y += y_add;
		}
	}
}

void Game::antimove(Move &m){
	if (m.Place_Move){
		GameBoard[m.x][m.y].Stack.pop_front();
		if (m.p.second == Black) GameBoard[m.x][m.y].Num_Black -= 1;
		else GameBoard[m.x][m.y].Num_White -= 1;
	}else{
		Position &current_p = GameBoard[m.x][m.y];
		int y_add = (m.Direction == '>') ? 1 : ((m.Direction == '<') ? -1 : 0);
		int x_add = (m.Direction == '+') ? 1 : ((m.Direction == '-') ? -1 : 0);
		int x = m.x + x_add;
		int y = m.y + y_add;
		for(auto &l : *m.Drops){
			auto &S = GameBoard[x][y].Stack;
			// for(auto itr = GameBoard[x][y].Stack.begin()+l-1; itr != GameBoard[x][y].Stack.begin(); --itr){
			for(int pos = l-1; pos >= 0; --pos){
				current_p.Stack.push_front(S[pos]);
				if(S[pos].second == Black){ 
					++current_p.Num_Black;
					--GameBoard[x][y].Num_Black;
				}
				else{ 
					++current_p.Num_White;
					--GameBoard[x][y].Num_White;
				}
			}
			for(int pos=0; pos<l; ++pos) GameBoard[x][y].Stack.pop_front();
			x += x_add;
			y += y_add;
		}
	}
}

std::tuple<int,int,int,int> Game::GetStackable(int x, int y)
{
	int l = 0;
	int r = 0;
	int u = 0;
	int d = 0;
	while (x-d-1 >= 0 && GameBoard[x-d-1][y].stackable())
		d ++;
	while (x+u+1 < size && GameBoard[x+u+1][y].stackable())
		u++;
	while (y-r-1 >=0 && GameBoard[x][y-r-1].stackable())
		r ++;
	while (y+l+1 < size && GameBoard[x][y+l+1].stackable())
		l ++;
	return make_tuple(l,r,u,d);
}

void Game::generate_valid_moves(Player_Type player, multimap<eval_type,Move> &moves){
	if((player == Black && p_black.CapsLeft != 0) || (player == White && p_white.CapsLeft != 0))
	for(int i=0; i<size; ++i){
		for(int j=0; j<size; ++j){
			if(GameBoard[i][j].empty()){
				Move m(i, j, piece(Cap,player));
				makemove(m);
				moves.insert(make_pair(eval(), m));
				antimove(m);
			}
		}
	}
	// placing caps done.
	if((player==Black && p_black.StonesLeft != 0) || (player==White && p_white.StonesLeft != 0))
	for(int i=0; i<size; ++i){
		for(int j=0; j<size; ++j){
			if(GameBoard[i][j].empty()){
				// place Flat/Stand
				Move m1(i, j, piece(Flat,player));
				Move m2(i, j, piece(Stand,player));
				makemove(m1);
				moves.insert(make_pair(eval(), m1));
				antimove(m1);
				makemove(m2);
				moves.insert(make_pair(eval(), m2));
				antimove(m2);
			}
			else
			{
				if (GameBoard[i][j].top_piece().second == White)
					printf("%d %d White\n", i, j);
				else
					printf("%d %d Black\n", i, j);
				 if (GameBoard[i][j].top_piece().second == player)
				{
					// all possible stack moves.
					int shiftmax = std::min((int)size, (int)GameBoard[i][j].Stack.size()); // max pieces.
					printf("%d %d Stack mera hai, shiftmax = %d \n", i, j, shiftmax);
					for (int i1 = 1 ; i1 <= shiftmax ; i1 ++)
					{
						// how many stackable in each dirn?
						tuple<int,int,int,int> range = GetStackable(i,j);
						printf("%d %d %d %d\n", get<0>(range), get<1>(range), get<2>(range), get<3>(range));
						for (int m = 1 ; m <= std::get<0>(range) ; m ++)
						{
							// left
							for (auto &d : AllPerms[i1][m])
							{
								Move ml(i,j,'<',&d);
								makemove(ml);
								moves.insert(make_pair(eval(),ml));
								antimove(ml);
							}
						}
						for (int m = 1 ; m <= std::get<1>(range) ; m ++)
						{
							// right
							for (auto &d : AllPerms[i1][m])
							{
								Move mr(i,j,'>',&d);
								makemove(mr);
								moves.insert(make_pair(eval(),mr));
								antimove(mr);
							}
						}

						for (int m = 1 ; m <= std::get<2>(range) ; m ++)
						{
							// up
							for (auto &d : AllPerms[i1][m])
							{
								Move mu(i,j,'+',&d);
								makemove(mu);
								moves.insert(make_pair(eval(),mu));
								antimove(mu);
							}

						}
						for (int m = 1 ; m <= std::get<3>(range) ; m ++)
						{
							// down
							for (auto &d : AllPerms[i1][m])
							{
								Move md(i,j,'-',&d);
								makemove(md);
								moves.insert(make_pair(eval(),md));
								antimove(md);
							}
						}
					}
				}
			}
		}
	}
}


