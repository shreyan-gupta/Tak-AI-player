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
	weight[0] = 10000;
	weight[1] = 1;
	weight[2] = 1;
	weight[3] = 1;
}

string Game::to_string()
{
	string str = "";
	for (int i = 0 ; i < size ; i ++){
		for (int j = 0 ; j < size ; j ++){
			str += GameBoard[i][j].to_string() + "_"; // this is a position.
		}
	}
	return str;
}

eval_type Game::eval(){
	eval_type e = 0;
	for(int i=0; i< 4; ++i){
		e += weight[i] * CALL_MEMBER_FN(this, f[i]) ();
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
		if(player == White && opponent_move > best_move){
			best_move = opponent_move;
		}else if(player == Black && opponent_move < best_move){
			best_move = opponent_move;
		}
	}
}

void Game::UpdatePlayer(Player_Type p_type, Move &m, bool anti){
	Player &p = (p_type == Black)? p_black : p_white;
	bool is_cap = (m.p.first == Cap);
	bool place = (m.Place_Move);
	if(!anti){
		if(place){
			if(is_cap){
				p.x = m.x;
				p.y = m.y;
				p.CapsLeft = false;
			}else{
				p.StonesLeft -= 1;
			}
		}else{
			int x_new = m.x + m.Drops->size() * ((m.Direction == '+') ? 1 : ((m.Direction == '-') ? -1 : 0));
			int y_new = m.y + m.Drops->size() * ((m.Direction == '>') ? 1 : ((m.Direction == '<') ? -1 : 0));
			if(is_cap){
				p.x = x_new;
				p.y = y_new;
			}
			if(m.CapMove){
				GameBoard[x_new][y_new].Stack.front().first = Flat;
			}
		}
	}else{
		if(place){
			if(is_cap){
				p.x = -1;
				p.y = -1;
				p.CapsLeft = true;
			}else{
				p.StonesLeft += 1;
			}
		}else{
			int x_new = m.x + m.Drops->size() * ((m.Direction == '+') ? 1 : ((m.Direction == '-') ? -1 : 0));
			int y_new = m.y + m.Drops->size() * ((m.Direction == '>') ? 1 : ((m.Direction == '<') ? -1 : 0));
			if(is_cap){
				p.x = m.x;
				p.y = m.y;
			}
			if(m.CapMove){
				GameBoard[x_new][y_new].Stack[1].first = Stand;
			}
		}
	}
}

void Game::makemove(Move &m)
{
	Player &p = (m.p.second == Black)? p_black : p_white;
	UpdatePlayer(m.p.second,m,false);
	if (m.Place_Move){
		// x,y pe posn mein push kardo. (stack must be empty abhi.)
		GameBoard[m.x][m.y].Stack.push_front(m.p);
		if(m.p.second == Black) GameBoard[m.x][m.y].Num_Black += 1;
		else GameBoard[m.x][m.y].Num_White += 1;

	}
	else{
		// move stack!
		vector<int> &d = *m.Drops;
		char dirn = m.Direction;
		int y_add = (dirn == '>') ? 1 : ((dirn == '<') ? -1 : 0);
		int x_add = (dirn == '+') ? 1 : ((dirn == '-') ? -1 : 0);
		int drop_x = m.x + x_add;
		int drop_y = m.y + y_add;

		std::deque<Piece> &mainstack = GameBoard[m.x][m.y].Stack;

		for (int i = 0 ; i < d.size() ; i ++)
		{
			int num_drops = d[i];
			for (int j = num_drops - 1 ; j > -1 ; j --)
			{
				Piece jth = mainstack[mainstack.size() - 1 - j];
				// printf("%d %d %d %d %d Pushing piece to front \n", drop_x, drop_y, m.x, m.y, mainstack.size());
				// if (jth.second == White)
					// cout << "White pushed to front \n";
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
	Player &p = (m.p.second == Black)? p_black : p_white;
	UpdatePlayer(m.p.second,m,true);
	if (m.Place_Move){
		GameBoard[m.x][m.y].Stack.pop_front();
		if(m.p.second == Black) GameBoard[m.x][m.y].Num_Black -= 1;
		else GameBoard[m.x][m.y].Num_White -= 1;
	}
	else{
		Position &current_p = GameBoard[m.x][m.y];
		int y_add = (m.Direction == '>') ? 1 : ((m.Direction == '<') ? -1 : 0);
		int x_add = (m.Direction == '+') ? 1 : ((m.Direction == '-') ? -1 : 0);
		int x = m.x + x_add;
		int y = m.y + y_add;
		for(auto &l : *m.Drops){
			auto &S = GameBoard[x][y].Stack;
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

void Game::GetStackable(int x, int y, bool cap, vector<int> &result){
	int l = 0;
	int r = 0;
	int u = 0;
	int d = 0;
	while (x-d-1 >= 0 && GameBoard[x-d-1][y].stackable())	d ++;
	while (x+u+1 < size && GameBoard[x+u+1][y].stackable())	u ++;
	while (y-r-1 >=0 && GameBoard[x][y-r-1].stackable())	r ++;
	while (y+l+1 < size && GameBoard[x][y+l+1].stackable())	l ++;
	if (cap){
		// cout << "capable breh! \n";
		if (x-d-1 >= 0 && GameBoard[x-d-1][y].capable())	d ++;	else d = -1;
		if (x+u+1 < size && GameBoard[x+u+1][y].capable())	u ++;	else u = -1;
		if (y-r-1 >= 0 && GameBoard[x][y-r-1].capable())	r ++;	else r = -1;
		if (y+l+1 < size && GameBoard[x][y+l+1].capable())	l ++;	else l = -1;
	}
	result[0] = l;
	result[1] = r;
	result[2] = u;
	result[3] = d;
	// return make_tuple(l,r,u,d);
}



void Game::generate_valid_moves(Player_Type player, multimap<eval_type,Move> &moves){
	Player &p = (player == Black) ? p_black : p_white;
	if(p.CapsLeft){
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
	}else{
		// cap stone on board!
		vector<int> range(4);
		// tuple<int,int,int,int> range = GetStackable(p.x, p.y, true);
		GetStackable(p.x, p.y, true, range);
		int shiftmax = std::min((int)size,(int)GameBoard[p.x][p.y].Stack.size());

		printf("Stackable %d %d %d %d\n", range[0], range[1], range[2], range[3]);
		// cap move :
		int i = p.x;
		int j = p.y;

		for (int i1 = 1 ; i1 <= shiftmax ; i1 ++){
			char dir[4] = {'<', '>', '+', '-'};
			for(int r=0; r<4; ++r){
				// printf("r=%d range_r=%d\n",r,range[r]);
				if(range[r] != -1)
				for(auto &d : AllPerms[i1][range[r]]){
					if(d.back() == 1){
						Move m(i,j,dir[r],&d);
						m.CapMove = true;
						makemove(m);
						moves.insert(make_pair(eval(),m));
						antimove(m);
					}
				}
			}


			// if(std::get<0>(range) != -1)
			// for (auto &d : AllPerms[i1][std::get<0>(range)]){
			// 	if (d.back() == 1){
			// 		Move ml(i,j,'<',&d);
			// 		ml.CapMove = true;
			// 		makemove(ml);
			// 		moves.insert(make_pair(eval(),ml));
			// 		antimove(ml);
			// 	}
			// }
			// if(std::get<1>(range) != -1)
			// for (auto &d : AllPerms[i1][std::get<1>(range)]){
			// 	if (d.back() == 1){
			// 		Move ml(i,j,'>',&d);
			// 		ml.CapMove = true;
			// 		makemove(ml);
			// 		moves.insert(make_pair(eval(),ml));
			// 		antimove(ml);
			// 	}
			// }
			// if(std::get<2>(range) != -1)
			// for (auto &d : AllPerms[i1][std::get<2>(range)]){
			// 	if (d.back() == 1){
			// 		Move ml(i,j,'+',&d);
			// 		ml.CapMove = true;
			// 		makemove(ml);
			// 		moves.insert(make_pair(eval(),ml));
			// 		antimove(ml);
			// 	}
			// }
			// if(std::get<3>(range) != -1)
			// for (auto &d : AllPerms[i1][std::get<3>(range)]){
			// 	if (d.back() == 1){
			// 		Move ml(i,j,'-',&d);
			// 		ml.CapMove = true;
			// 		makemove(ml);
			// 		moves.insert(make_pair(eval(),ml));
			// 		antimove(ml);
			// 	}
			// }
		}
	}

	// placing caps done.
	if(p.StonesLeft != 0)
	
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
			}else if (GameBoard[i][j].top_piece().second == player) {
				// all possible stack moves.
				cout << "stack move \n";
				int shiftmax = std::min((int)size, (int)GameBoard[i][j].Stack.size()); // max pieces.
				for (int i1 = 1 ; i1 <= shiftmax ; i1 ++){
					// how many stackable in each dirn?
					vector<int> range(4);
 					// tuple<int,int,int,int> range = GetStackable(i,j,false);
 					GetStackable(i,j,false,range);
					printf("Bla %d %d %d %d\n", range[0], range[1], range[2], range[3]);
					
					char dir[] = {'<', '>', '+', '-'};
					for(int r=0; r<4; ++r){
						for (int m=1; m<=range[r]; ++m){
							for (auto &d : AllPerms[i1][m]){
								Move mv(i,j,dir[r],&d);
								makemove(mv);
								moves.insert(make_pair(eval(),mv));
								antimove(mv);
							}
						}
					}

					// for (int m = 1 ; m <= std::get<0>(range) ; m ++){
					// 	// left
					// 	for (auto &d : AllPerms[i1][m])
					// 	{
					// 		Move ml(i,j,'<',&d);
					// 		makemove(ml);
					// 		moves.insert(make_pair(eval(),ml));
					// 		antimove(ml);
					// 	}
					// }

					// for (int m = 1 ; m <= std::get<1>(range) ; m ++)
					// {
					// 	// right
					// 	for (auto &d : AllPerms[i1][m])
					// 	{
					// 		Move mr(i,j,'>',&d);
					// 		makemove(mr);
					// 		moves.insert(make_pair(eval(),mr));
					// 		antimove(mr);
					// 	}
					// }

					// for (int m = 1 ; m <= std::get<2>(range) ; m ++)
					// {
					// 	// up
					// 	for (auto &d : AllPerms[i1][m])
					// 	{
					// 		Move mu(i,j,'+',&d);
					// 		makemove(mu);
					// 		moves.insert(make_pair(eval(),mu));
					// 		antimove(mu);
					// 	}

					// }
					// for (int m = 1 ; m <= std::get<3>(range) ; m ++)
					// {
					// 	// down
					// 	for (auto &d : AllPerms[i1][m])
					// 	{
					// 		Move md(i,j,'-',&d);
					// 		makemove(md);
					// 		moves.insert(make_pair(eval(),md));
					// 		antimove(md);
					// 	}
					// }
				}
			}
		}
	}
}


