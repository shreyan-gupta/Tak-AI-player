#include "Header.h"
using namespace Types;

Player::Player(bool player_type, int pieces, int caps){
	type = player_type;
	StonesLeft = pieces;
	CapsLeft = caps;
}

Move::Move(int x, int y, Piece p){
	this->Place_Move = true;
	this->x = x;
	this->y = y;
	this->p = p;
	this->Direction;
	this->Drops = vector<int>();
}

Move::Move(int x, int y, char d, vector<int> &v){
	this->Place_Move = false;
	this->x = x;
	this->y = y;
	this->Direction = d;
	this->Drops = v;
}

inline Piece piece(Stone s, bool p){
	return make_pair(s,p);
}

inline Piece Position::top_piece(){
	return Stack.front();
}

inline bool Position::empty(){
	return Stack.empty();
}

inline bool Position::stackable(){
	if(empty) return true;
	else if(Stack.front().first == Flat) return true;
	else return false;
}

string Position::to_string(){
	int val=0;
	int mul = 1;
	string s = "";
	for(auto &i : Stack){
		if(i.second == Black) val += mul;
		mul = mul << 1;
	}
	val += mul;
	s += std::to_string(val);
	if(!empty()){
		if(Stack.back().first == Flat) s += "F";
		else if(Stack.back().first == Stand) s+= "S";
		else s += "C";
	}
	return s;
}

Game::Game(int size){
	this->size = size;
	GameBoard = vector< vector<Position> >(size, vector<Position>(size));
	p_white = Player(White, 100, 1);
	p_black = Player(Black, 100, 1);
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

eval_type Game::eval()
{
	// TODO
}

pair<eval_type, Move> Game::decide_move(bool player, int depth, int max_depth){
	
	multimap <eval_type, Move> allmoves;
	generate_valid_moves(player, allmoves);
	
	if(depth == max_depth) return make_pair(allmoves.begin().first, allmoves.begin().second);
	
	pair<eval_type, Move> best_move = make_pair(player?-99999999:9999999, Move(-1,-1, make_pair(Flat, player)));
	//iterate over all valid moves
	for (auto &i : allmoves){
		// makemove.
		// ask !player to decide his best move
		// make anti move.
		
		makemove(i.second);
		auto opponent_move = decide_move(!player, depth+1, max_depth);
		if(player && opponent_move > best_move){
			best_move = opponent_move;
		}else if(!player && opponent_move < best_move){
			best_move = opponent_move;
		}
	}
	return best_move;
}


void Game::makemove(Move &m)
{
	// TODO
	if (m.Place_Move)
	{
		// x,y pe posn mein push kardo. (stack must be empty abhi.)
			// push WHAT?? 
			GameBoard[m.x][m.y].Stack.push_back(m.p);
			if (m.p.second == Black)
				GameBoard[m.x][m.y].Num_Black += 1;
			else
				GameBoard[m.x][m.y].Num_White += 1;
	}
	else
	{
		// move/anti?
		vector<int> & d = m.Drops;
		char dirn = m.Direction;
		int x_add = (dirn == '>') ? 1 : ((dirn == '<') ? -1 : 0);
		int y_add = (dirn == '+') ? 1 : ((dirn == '-') ? -1 : 0);
		int drop_x = m.x + x_add;
		int drop_y = m.y + y_add;

		std::deque<Piece> & mainstack = GameBoard[m.x][m.y].Stack;

		for (int i = 0 ; i < d.size() ; i ++)
		{
			int num_drops = d[i];
			for (int j = num_drops - 1 ; j > -1 ; j --)
			{
				Piece jth = mainstack[mainstack.size() - 1 - j];
				GameBoard[drop_x][drop_y].Stack.push_back(jth);
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
				mainstack.pop_back();
			}
			drop_x += x_add;
			drop_y += y_add;
		}
	}
}

void Game::antimove(Move &m){
	if (m.Place_Move){
		GameBoard[m.x][m.y].Stack.pop_back();
		if (m.p.second == Black) GameBoard[m.x][m.y].Num_Black -= 1;
		else GameBoard[m.x][m.y].Num_White -= 1;
	}else{
		Position &current_p = GameBoard[m.x][m.y];
		int x_add = (m.Direction == '>') ? 1 : ((m.Direction == '<') ? -1 : 0);
		int y_add = (m.Direction == '+') ? 1 : ((m.Direction == '-') ? -1 : 0);
		int x = m.x + x_add;
		int y = m.y + y_add;
		for(auto &l : m.Drops){
			auto itr = GameBoard[x][y].Stack.end() - l;
			while(itr != GameBoard[x][y].Stack.end()){
				current_p.Stack.push_back(*itr);
				if(itr->second == Black){ 
					++current_p.Num_Black;
					--GameBoard[x][y].Num_Black;
				}
				else{ 
					++current_p.Num_White;
					--GameBoard[x][y].Num_White;
				}
				itr = GameBoard[x][y].Stack.erase(itr);
			}
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
	while (x-l-1 >= 0 && GameBoard[x-l-1][y].stackable())
		l --;
	while (x+r+1 < size && GameBoard[x+r+1][y].stackable())
		r++;
	while (y-d-1 >=0 && GameBoard[x][y-d-1].stackable())
		d --;
	while (y+u+1 < size && GameBoard[x][y+u+1].stackable())
		u ++;
	return make_tuple(l,r,u,d);
}

void Game::generate_valid_moves(bool player, multimap<eval_type,Move> &move){
	if(player == Black && p_black.CapsLeft != 0 || player == White && p_white.CapsLeft != 0)
	for(int i=0; i<size; ++i){
		for(int j=0; j<size; ++j){
			if(GameBoard[i][j].empty()){
				Move m(i, j, piece(Cap,player));
				makemove(m);
				moves.insert(eval(), m);
				antimove(m);
			}
		}
	}
	// placing caps done.
	if(player==Black && p_black.StonesLeft != 0 || player==White && p_white.StonesLeft != 0)
	for(int i=0; i<size; ++i){
		for(int j=0; j<size; ++j){
			if(GameBoard[i][j].empty()){
				// place Flat/Stand
				Move m1(i, j, piece(Flat,player));
				Move m2(i, j, piece(Stand,player));
				makemove(m1);
				moves.insert(eval(), m1);
				antimove(m1);
				makemove(m2);
				moves.insert(eval(), m2);
				antimove(m2);
			}
			else if (GameBoard[i][j].top_piece().second == player)
			{
				// all possible stack moves.
				int shiftmax = std::min(size,GameBoard[i][j].Stack.size()); // max pieces.
				for (int i1 = 1 ; i1 <= shiftmax ; i1 ++)
				{
					// how many stackable in each dirn?
					tuple<int,int,int,int> range = GetStackable(i,j);
					for (int m = 1 ; m <= std::get<0>(range) ; m ++)
					{
						// left
						for (auto &d : AllPerms[i1][m])
						{
							Move ml(i,j,'<',d);
							makemove(ml);
							moves.insert(eval(),ml);
							antimove(ml);
						}
					}
					for (int m = 1 ; m <= std::get<1>(range) ; m ++)
					{
						// right
						for (auto &d : AllPerms[i1][m])
						{
							Move mr(i,j,'>',d);
							makemove(mr);
							moves.insert(eval(),mr);
							antimove(mr);
						}
					}

					for (int m = 1 ; m <= std::get<2>(range) ; m ++)
					{
						// up
						for (auto &d : AllPerms[i1][m])
						{
							Move mu(i,j,'+',d);
							makemove(mu);
							moves.insert(eval(),mu);
							antimove(mu);
						}

					}
					for (int m = 1 ; m <= std::get<3>(range) ; m ++)
					{
						// down
						for (auto &d : AllPerms[i1][m])
						{
							Move md(i,j,'-',d);
							makemove(md);
							moves.insert(eval(),md);
							antimove(md);
						}

					}
				}
			}
		}
	}
}


