#include "Header.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))

using namespace Types;

eval_type E_MIN = -99999999999;
eval_type E_MAX = +99999999999;
Piece NULL_PIECE = make_pair(Flat, false);
Move NULL_MOVE = Move(-1, -1, NULL_PIECE);

Game::Game(int size, int pieces) : p_white(Player(White, pieces)), p_black(Player(Black, pieces))
{
	this->size = size;
	GameBoard = vector< vector<Position> >(size, vector<Position>(size));

	f[0] = &Game::feature0;
	f[1] = &Game::feature1;
	f[2] = &Game::feature2;
	f[3] = &Game::feature3;
	f[4] = &Game::feature4;
	f[5] = &Game::feature5;
	
	float piece_factor = 1;
	float nbr_factor = 0.7;
	float top_factor = 2;


	w[0] = 1000000;
	w[15] = 900000;

	w[1] = 0.35;

	w[4] = piece_factor * 4;
	w[5] = piece_factor * 1;	// original 2

	w[6] = top_factor * 11;
	w[7] = top_factor * 5;
	w[8] = top_factor * 11;

	w[9]  = nbr_factor * 9.5;
	w[10] = nbr_factor * 5;
	w[11] = nbr_factor * 8.5;
	w[12] = nbr_factor * -2;
	w[13] = nbr_factor * 3;
	w[14] = nbr_factor * -2;
	w[2]  = nbr_factor * -4;
	w[3]  = nbr_factor * -5;

	w[16] = 7;

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
	string s = to_string();
	auto ptr = duplicates.find(s);
	if(ptr != duplicates.end()) return ptr->second;
	eval_type e = 0;
	for(int i=0; i<6; ++i){
		e += CALL_MEMBER_FN(this, f[i]) ();
	}
	duplicates[s] = e;
	return e;
}

// void n_tabs(int n){
// 	for(int i=0; i<n; ++i) cerr << "  ";
// }

void Game::decide_move(Eval_Move &best_move, bool player, int depth, int max_depth, eval_type alpha, eval_type beta){
	
	multimap <eval_type, Move> allmoves;
	generate_valid_moves(player, allmoves);

	// n_tabs(depth); fprintf(stderr, "Plyr:%d Depth:%d\n",player,depth);
	// for(auto &i : allmoves){
	// 		cerr << i.first << " XXXXX " << i.second.to_string() << endl;
	// }
	
	if(depth == max_depth){
		if(player == White){
			auto ptr = allmoves.rbegin();
			best_move.e = ptr->first;
			best_move.m = ptr->second;
		}else{
			auto ptr = allmoves.begin();
			best_move.e = ptr->first;
			best_move.m = ptr->second;
		}
		// n_tabs(depth); fprintf(stderr, "%s : Best move\n",best_move.m.to_string().c_str());
		return;
	}

	Eval_Move opponent_move;
	if(player == White) best_move.e = E_MIN;
	else best_move.e = E_MAX;

	int index = 0;
	int best_index = 0;

	if(player == White){
		auto best_ptr = allmoves.rbegin();
		if (best_ptr->first > w[0]/2){
			best_move.e = best_ptr->first;
			best_move.m = best_ptr->second;
			if(depth < 2) fprintf(stderr, "White ka final move %s at depth %d\n",best_move.m.to_string().c_str(), depth);
			return;
		}
		bool x = true;
		for(auto ptr = allmoves.rbegin(); x && ptr != allmoves.rend(); ++ptr){
			++index;
			makemove(ptr->second);
			decide_move(opponent_move, !player, depth+1, max_depth, alpha, beta);
			if(opponent_move.e > best_move.e){
					if (abs(opponent_move.e + 1000000) < 10000)
						fprintf(stderr, "%f %s : Path Move Black \n", opponent_move.e, opponent_move.m.to_string().c_str());
				best_index = index;
				best_move.e = opponent_move.e;
				best_ptr = ptr;
					// if(depth < 2){
					// 	fprintf(stderr, "d %d index %d White Predicted eval : %f Actual eval : %f Move : %s\n", depth, index, ptr->first, best_move.e, ptr->second.to_string().c_str());
					// }
			}
			antimove(ptr->second);
			alpha = max(alpha, best_move.e);
			if(best_move.e >= beta){
				best_move.m = best_ptr->second;
				// fprintf(stderr, "Prune %d depth %d\n",index,depth);
				return;
			}
			x = (index < best_index + 5 || best_move.e < -w[0]/2);
			// if(best_move.e < -w[0]/2){
			// 	fprintf(stderr, "Further Exploring %d at depth %d\n",index,depth);
			// }
		}
		best_move.m = best_ptr->second;

		sum_index[depth] += best_index;
		++count_index[depth];
		max_index[depth] = max(best_index, max_index[depth]);
	}
	else{
		auto best_ptr = allmoves.begin();
		if (best_ptr->first < -w[0]/2){
			best_move.e = best_ptr->first;
			best_move.m = best_ptr->second;
			if(depth < 2) fprintf(stderr, "Black ka final move %s at depth %d\n",best_move.m.to_string().c_str(), depth);
			return;
		}
		bool x = true;
		for(auto ptr = allmoves.begin(); x && ptr != allmoves.end(); ++ptr){
			++index;
			makemove(ptr->second);
			decide_move(opponent_move, !player, depth+1, max_depth, alpha, beta);
			if(opponent_move.e < best_move.e){
					if (abs(opponent_move.e - 1000000) < 10000)
						fprintf(stderr, "%f %s : Path Move White \n", opponent_move.e, opponent_move.m.to_string().c_str());
				best_index = index;
				best_move.e = opponent_move.e;
				best_ptr = ptr;
					// if(depth < 2){
					// 	fprintf(stderr, "d %d index %d Black Predicted eval : %f Actual eval : %f Move : %s\n", depth, index, ptr->first, best_move.e, ptr->second.to_string().c_str());
					// }
			}
			antimove(ptr->second);
			beta = min(beta, best_move.e);
			if(best_move.e <= alpha){
				best_move.m = best_ptr->second;
				// fprintf(stderr, "Prune %d depth %d\n",index,depth);
				return;
			}
			x = (index < best_index + 5 || best_move.e > w[0]/2);
			// if(best_move.e > w[0]/2){
			// 	fprintf(stderr, "Further Exploring %d at depth %d\n",index,depth);
			// }
		}
		best_move.m = best_ptr->second;
		sum_index[depth] += best_index;
		++count_index[depth];
		max_index[depth] = max(best_index, max_index[depth]);
	}
}

void Game::make_opponent_move(string s, bool player)
{
	char first = s.at(0);
	Move m;
	m.x = Size - (s.at(2) - '0');
	m.y = s.at(1) - 'a';
	if (first == 'F' || first == 'S' || first == 'C')
	{
		m.Place_Move = true;
		switch (first)
		{
			case ('F'):
				m.p = piece(Flat,player);
				break;
			case ('S'):
				m.p = piece(Stand,player);
				break;
			case ('C'):
				m.p = piece(Cap,player);
				break;
		}
		makemove(m);
	}
	else
	{
		m.Place_Move = false;
		m.Direction = s.at(3);
		if (s.at(3) == '+')
			m.Direction = '-';
		else if (s.at(3) == '-')
			m.Direction = '+';
		vector<int> drops (s.length() - 4,1);
		fprintf(stderr, "%d\n", s.length() - 4);
		for (int i = 0 ; i < s.length() - 4 ; i ++)
			drops[i] = (int)(s.at(i+4) - '0');
		m.Drops = &drops;
		int x_new = m.x + m.Drops->size() * ((m.Direction == '+') ? 1 : ((m.Direction == '-') ? -1 : 0));
		int y_new = m.y + m.Drops->size() * ((m.Direction == '>') ? 1 : ((m.Direction == '<') ? -1 : 0));		printVec(*m.Drops);
		if (GameBoard[m.x][m.y].top_piece().first == Cap)
		{
			Position &p = GameBoard[x_new][y_new];
			if (!p.empty() && p.top_piece().first == Stand)
				m.CapMove = true;
		}
		makemove(m);
		// cerr << "Oppo move is :::: " << m.to_string() << endl;
	}
}

void Game::UpdatePlayer(Player_Type p_type, Move &m, bool anti){
	Player &p = (p_type == White) ? p_white : p_black;
	bool is_cap = (m.p.first == Cap);
	bool place = (m.Place_Move);
	if(!anti){
		if(place){
			if(is_cap){
				p.x = m.x;
				p.y = m.y;
				p.CapLeft = false;
			}else{
				p.StonesLeft -= 1;
			}
		}else{
			int x_new = m.x + m.Drops->size() * ((m.Direction == '+') ? 1 : ((m.Direction == '-') ? -1 : 0));
			int y_new = m.y + m.Drops->size() * ((m.Direction == '>') ? 1 : ((m.Direction == '<') ? -1 : 0));
			if(GameBoard[m.x][m.y].top_piece().first == Cap){
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
				p.CapLeft = true;
			}else{
				p.StonesLeft += 1;
			}
		}else{
			int x_new = m.x + m.Drops->size() * ((m.Direction == '+') ? 1 : ((m.Direction == '-') ? -1 : 0));
			int y_new = m.y + m.Drops->size() * ((m.Direction == '>') ? 1 : ((m.Direction == '<') ? -1 : 0));
			if(GameBoard[x_new][y_new].top_piece().first == Cap){
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
	// Player &p = (m.p.second == Black)? p_black : p_white;
	if (m.Place_Move){
		UpdatePlayer(m.p.second,m,false);
			// if(!GameBoard[m.x][m.y].empty()){
			// 	fprintf(stderr, "ERRRRROOOOOORRRRRRRRRR!!!!!!!\n");
			// 	fprintf(stderr, "Game Board : %s\n", to_string().c_str());
			// 	fprintf(stderr, "Move : %s\n", m.to_string().c_str());
			// }
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
		int drop_x = m.x + x_add*(m.Drops->size());
		int drop_y = m.y + y_add*(m.Drops->size());

		UpdatePlayer(GameBoard[m.x][m.y].top_piece().second,m,false);
		auto &mainstack = GameBoard[m.x][m.y].Stack;

		for (int i = d.size() - 1; i >=  0 ; i--)
		{
			for (int j = d[i] - 1; j > -1; j--)
			{
				GameBoard[drop_x][drop_y].Stack.push_front(mainstack[j]);
				if (mainstack[j].second == Black)
					GameBoard[drop_x][drop_y].Num_Black += 1;
				else
					GameBoard[drop_x][drop_y].Num_White += 1;
			}
			// pop all these!
			for (int j = 0; j < d[i]; j++)
			{
				if (mainstack.front().second == Black)
					GameBoard[m.x][m.y].Num_Black -= 1;
				else
					GameBoard[m.x][m.y].Num_White -= 1;
				mainstack.pop_front();
			}
			drop_x -= x_add;
			drop_y -= y_add;
		}
	}
}

void Game::antimove(Move &m){
	// Player &p = (m.p.second == Black)? p_black : p_white;
	if (m.Place_Move){
		UpdatePlayer(m.p.second,m,true);
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
		UpdatePlayer(GameBoard[m.x + m.Drops->size() * x_add][m.y + m.Drops->size() * y_add].top_piece().second,m,true);
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
	while (y+r+1 < size && GameBoard[x][y+r+1].stackable())	r ++;
	while (y-l-1 >=0 && GameBoard[x][y-l-1].stackable())	l ++;
	if (cap){
		if (x-d-1 >= 0 && GameBoard[x-d-1][y].capable())	d ++;	else d = -1;
		if (x+u+1 < size && GameBoard[x+u+1][y].capable())	u ++;	else u = -1;
		if (y-l-1 >= 0 && GameBoard[x][y-l-1].capable())	l ++;	else l = -1;
		if (y+r+1 < size && GameBoard[x][y+r+1].capable())	r ++;	else r = -1;
	}
	result[0] = l;
	result[1] = r;
	result[2] = u;
	result[3] = d;
	// fprintf(stderr, "coord %d %d direction <%d >%d -%d +%d\n",x,y,l,r,u,d);
}



void Game::generate_valid_moves(Player_Type player, multimap<eval_type,Move> &moves){
	Player &p = (player == Black) ? p_black : p_white;
	if(p.CapLeft){
		for(int i=0; i<size; ++i){
			for(int j=0; j<size; ++j){
				if(GameBoard[i][j].empty()){
					Move m(i, j, piece(Cap,player));
						// string s1 = to_string();
					makemove(m);
					moves.insert(make_pair(eval(), m));
					antimove(m);
						// string s2 = to_string();
						// if(s1.compare(s2) != 0){
						// 	fprintf(stderr, "ERRRRROOOOOORRRRRRRRRR!!!!!!!!!\n");
						// 	fprintf(stderr, "Board is %s\n",s1.c_str());
						// 	fprintf(stderr, "Board is %s\n",s2.c_str());
						// 	fprintf(stderr, "At move %s\n",m.to_string().c_str());
						// }
				}
			}
		}
	}else{
		// cap stone on board!
		vector<int> range(4);
		GetStackable(p.x, p.y, true, range);
		int shiftmax = std::min((int)size,(int)GameBoard[p.x][p.y].Stack.size());

		// if(p.x == 1 && p.y == 1)
		// 	fprintf(stderr, "Stackable %d %d %d %d\n", range[0], range[1], range[2], range[3]);
		// cap move :
		int i = p.x;
		int j = p.y;

		for (int i1 = 1 ; i1 <= shiftmax ; i1 ++){
			char dir[4] = {'<', '>', '+', '-'};
			for(int r=0; r<4; ++r){
				if(range[r] != -1)
				for(auto &d : AllPerms[i1][range[r]]){
					if(d.back() == 1){
						Move m(i,j,dir[r],&d);
						m.CapMove = true;
							string s1 = to_string();
						makemove(m);
						moves.insert(make_pair(eval(), m));
						antimove(m);
							string s2 = to_string();
							if(s1.compare(s2) != 0){
								fprintf(stderr, "ERRRRROOOOOORRRRRRRRRR!!!!!!!!!\n");
								fprintf(stderr, "Board is %s\n",s1.c_str());
								fprintf(stderr, "Board is %s\n",s2.c_str());
								fprintf(stderr, "At move %s\n",m.to_string().c_str());
							}
					}
				}
			}
		}
	}

	// placing caps done.
	if(p.StonesLeft != 0)
	
	for(int i=0; i<size; ++i){
		for(int j=0; j<size; ++j){
			if(GameBoard[i][j].empty()){
				// place Flat/Stand
				Move m1(i, j, piece(Flat,player));
					// string s1 = to_string();
				makemove(m1);
				moves.insert(make_pair(eval(), m1));
				antimove(m1);
					// string s2 = to_string();
					// if(s1.compare(s2) != 0){
					// 	fprintf(stderr, "ERRRRROOOOOORRRRRRRRRR!!!!!!!!!\n");
					// 	fprintf(stderr, "Board is %s\n",s1.c_str());
					// 	fprintf(stderr, "Board is %s\n",s2.c_str());
					// 	fprintf(stderr, "At move %s\n",m1.to_string().c_str());
					// }
				Move m2(i, j, piece(Stand,player));
					// s1 = to_string();
				makemove(m2);
				moves.insert(make_pair(eval(), m2));
				antimove(m2);
					// s2 = to_string();
					// if(s1.compare(s2) != 0){
					// 	fprintf(stderr, "ERRRRROOOOOORRRRRRRRRR!!!!!!!!!\n");
					// 	fprintf(stderr, "Board is %s\n",s1.c_str());
					// 	fprintf(stderr, "Board is %s\n",s2.c_str());
					// 	fprintf(stderr, "At move %s\n",m2.to_string().c_str());
					// }
			
			}
			else if (GameBoard[i][j].top_piece().second == player) {
				// all possible stack moves.
				int shiftmax = std::min((int)size, (int)GameBoard[i][j].Stack.size()); // max pieces.
				for (int i1 = 1 ; i1 <= shiftmax ; i1 ++){
					// how many stackable in each dirn?
					vector<int> range(4);
 					GetStackable(i,j,false,range);
					
					char dir[] = {'<', '>', '+', '-'};
						// if (i==1 && j==1){
						// 	// fprintf(stderr, "Coordinates %d %d\n",i,j);
						// 	fprintf(stderr, "direction <%d >%d -%d +%d\n", range[0], range[1], range[2], range[3]);						
						// }
					for(int r=0; r<4; ++r){
						for (int m=1; m<=range[r]; ++m){
							for (auto &d : AllPerms[i1][m]){
								Move mv(i,j,dir[r],&d);
									// string s1 = to_string();
								makemove(mv);
									// string s3 = to_string();
								moves.insert(make_pair(eval(),mv));
								antimove(mv);
									// string s2 = to_string();
									// if(s1.compare(s2) != 0){
									// 	fprintf(stderr, "ERRRRROOOOOORRRRRRRRRR!!!!!!!!!\n");
									// 	fprintf(stderr, "Board now %s\n",s1.c_str());
									// 	fprintf(stderr, "Board after move %s\n",s3.c_str());
									// 	fprintf(stderr, "Board later %s\n",s2.c_str());
									// 	fprintf(stderr, "At move %s\n",mv.to_string().c_str());
									// }
							}
						}
					}
				}
			}
		}
	}
}


