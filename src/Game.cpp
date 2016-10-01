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
	weight[1] = 20;
	weight[2] = 15;
	weight[3] = 12;
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
	for(int i=0; i< 4; ++i){
		e += weight[i] * CALL_MEMBER_FN(this, f[i]) ();
	}
	duplicates[s] = e;
	return e;
}

void n_tabs(int n){
	for(int i=0; i<n; ++i) cout << "  ";
}

void Game::decide_move(Eval_Move &best_move, bool player, int depth, int max_depth){
	
	// n_tabs(depth); printf("Plyr:%d Depth:%d\n",player,depth);
	// n_tabs(depth); printf("%s\n", );

	multimap <eval_type, Move> allmoves;
	generate_valid_moves(player, allmoves);

	// n_tabs(depth); printf("Generated moves depth %d \n",depth);
	
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
		// n_tabs(depth); printf("%s : Best move\n",best_move.m.to_string().c_str());
		return;
	}

	Eval_Move opponent_move;
	if(player == White) best_move.e = E_MIN;
	else best_move.e = E_MAX;

	for (auto &i : allmoves){
		// n_tabs(depth); printf("%s\n",i.second.to_string().c_str());
		makemove(i.second);
		decide_move(opponent_move, !player, depth+1, max_depth);
		if(player == White && opponent_move > best_move){
			best_move = opponent_move;
		}else if(player == Black && opponent_move < best_move){
			best_move = opponent_move;
		}
		antimove(i.second);
	}
}

void Game::make_opponent_move(string s)
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
				m.p = piece(Flat,opponent_type);
				break;
			case ('S'):
				m.p = piece(Stand,opponent_type);
				break;
			case ('C'):
				m.p = piece(Cap,opponent_type);
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
		printf("%d\n", s.length() - 4);
		for (int i = 0 ; i < s.length() - 4 ; i ++)
			drops[i] = (int)(s.at(i+4) - '0');
		m.Drops = &drops;
		printVec(*m.Drops);
		makemove(m);
		cout << "Oppo move is :::: " << m.to_string() << endl;
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
		int drop_x = m.x + x_add*(m.Drops->size());
		int drop_y = m.y + y_add*(m.Drops->size());

		std::deque<Piece> &mainstack = GameBoard[m.x][m.y].Stack;

		for (int i = d.size() - 1 ; i >=  0 ; i--)
		{
			int num_drops = d[i];
			for (int j = num_drops - 1 ; j > -1 ; j --)
			{
				Piece jth = mainstack[j];
					// if (m.x == 2 && m.y == 1)
					// {
					// 	printf("%d %d %d, 2 1 se uthaya ye piece\n", jth.second, mainstack.size(), j);
					// }
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
			drop_x -= x_add;
			drop_y -= y_add;
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
	while (y-l-1 >=0 && GameBoard[x][y-l-1].stackable())	l ++;
	while (y+r+1 < size && GameBoard[x][y+r+1].stackable())	r ++;
	if (cap){
		// cout << "capable breh! \n";
		if (x-d-1 >= 0 && GameBoard[x-d-1][y].capable())	d ++;	else d = -1;
		if (x+u+1 < size && GameBoard[x+u+1][y].capable())	u ++;	else u = -1;
		if (y-l-1 >= 0 && GameBoard[x][y-l-1].capable())	l ++;	else l = -1;
		if (y+r+1 < size && GameBoard[x][y+r+1].capable())	r ++;	else r = -1;
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
						string s1 = to_string();
					makemove(m);
					moves.insert(make_pair(eval(), m));
					antimove(m);
						string s2 = to_string();
						if(s1.compare(s2) != 0){
							printf("ERRRRROOOOOORRRRRRRRRR!!!!!!!!!\n");
							printf("Board is %s\n",s1.c_str());
							printf("Board is %s\n",s2.c_str());
							printf("At move %s\n",m.to_string().c_str());
						}
				}
			}
		}
	}else{
		// cap stone on board!
		vector<int> range(4);
		// tuple<int,int,int,int> range = GetStackable(p.x, p.y, true);
		GetStackable(p.x, p.y, true, range);
		int shiftmax = std::min((int)size,(int)GameBoard[p.x][p.y].Stack.size());

		// printf("Stackable %d %d %d %d\n", range[0], range[1], range[2], range[3]);
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
							string s1 = to_string();
						makemove(m);
						moves.insert(make_pair(eval(), m));
						antimove(m);
							string s2 = to_string();
							if(s1.compare(s2) != 0){
								printf("ERRRRROOOOOORRRRRRRRRR!!!!!!!!!\n");
								printf("Board is %s\n",s1.c_str());
								printf("Board is %s\n",s2.c_str());
								printf("At move %s\n",m.to_string().c_str());
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
					string s1 = to_string();
				makemove(m1);
				moves.insert(make_pair(eval(), m1));
				antimove(m1);
					string s2 = to_string();
					if(s1.compare(s2) != 0){
						printf("ERRRRROOOOOORRRRRRRRRR!!!!!!!!!\n");
						printf("Board is %s\n",s1.c_str());
						printf("Board is %s\n",s2.c_str());
						printf("At move %s\n",m1.to_string().c_str());
					}
				Move m2(i, j, piece(Stand,player));
					s1 = to_string();
				makemove(m2);
				moves.insert(make_pair(eval(), m2));
				antimove(m2);
					s2 = to_string();
					if(s1.compare(s2) != 0){
						printf("ERRRRROOOOOORRRRRRRRRR!!!!!!!!!\n");
						printf("Board is %s\n",s1.c_str());
						printf("Board is %s\n",s2.c_str());
						printf("At move %s\n",m2.to_string().c_str());
					}
			
			}else if (GameBoard[i][j].top_piece().second == player) {
				// all possible stack moves.
				// cout << "stack move \n";
				int shiftmax = std::min((int)size, (int)GameBoard[i][j].Stack.size()); // max pieces.
				for (int i1 = 1 ; i1 <= shiftmax ; i1 ++){
					// how many stackable in each dirn?
					vector<int> range(4);
 					// tuple<int,int,int,int> range = GetStackable(i,j,false);
 					GetStackable(i,j,false,range);
					
					char dir[] = {'<', '>', '+', '-'};
						// if (GameBoard[i][j].top_piece().first == Cap){
						// 	printf("Coordinates %d %d\n",i,j);
						// 	printf("Cap direction <%d >%d +%d -%d\n", range[0], range[1], range[2], range[3]);						
						// }
					for(int r=0; r<4; ++r){
						for (int m=1; m<=range[r]; ++m){
							for (auto &d : AllPerms[i1][m]){
								Move mv(i,j,dir[r],&d);
									string s1 = to_string();
								makemove(mv);
									string s3 = to_string();
								moves.insert(make_pair(eval(),mv));
								antimove(mv);
									string s2 = to_string();
									if(s1.compare(s2) != 0){
										printf("ERRRRROOOOOORRRRRRRRRR!!!!!!!!!\n");
										printf("Board now %s\n",s1.c_str());
										printf("Board after move %s\n",s3.c_str());
										printf("Board later %s\n",s2.c_str());
										printf("At move %s\n",mv.to_string().c_str());
									}
							}
						}
					}
				}
			}
		}
	}
}


