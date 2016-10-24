#include "Game.h"

Game::Game(s_int s, s_int pieces) : p_white(Player(White, pieces)), p_black(Player(Black, pieces)){
	this->size = s;
	GameBoard = vector< vector<Position> >(size, vector<Position>(size));
	TTable = vector<unordered_map<string,Transposition> > (2);
}

string Game::to_string(){
	string str = "";
	for(auto &i : GameBoard){
		for(auto &j : i){
			str += j.to_string() + "_";
		}
	}
	return str;
}

// eval, decide_move pending

eval_type Game::eval()
{
	return 0;
}

void Game::UpdatePlayer(Player_Type p_type, Move &m, bool anti){
	Player &p = (p_type == White) ? p_white : p_black;
	bool is_cap = (toupper(m.piece) == 'C');
	bool place = (m.place_move);
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
			s_int x_new = m.x + m.drops->size() * ((m.direction == '+') ? 1 : ((m.direction == '-') ? -1 : 0));
			s_int y_new = m.y + m.drops->size() * ((m.direction == '>') ? 1 : ((m.direction == '<') ? -1 : 0));
			if(GameBoard[m.x][m.y].top_piece() == 'C'){
				p.x = x_new;
				p.y = y_new;
			}
			if(m.cap_move){
				if(GameBoard[x_new][y_new].player() == White)
					GameBoard[x_new][y_new].stack.back() = 'F';
				else 
					GameBoard[x_new][y_new].stack.back() = 'f';
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
			s_int x_new = m.x + m.drops->size() * ((m.direction == '+') ? 1 : ((m.direction == '-') ? -1 : 0));
			s_int y_new = m.y + m.drops->size() * ((m.direction == '>') ? 1 : ((m.direction == '<') ? -1 : 0));
			if(GameBoard[x_new][y_new].top_piece() == 'C'){
				p.x = m.x;
				p.y = m.y;
			}
			if(m.cap_move){
				auto &pos = GameBoard[x_new][y_new];
				if(pos.stack[pos.stack.size() - 2] < 95)
					pos.stack[pos.stack.size() - 2] = 'S';
				else 
					pos.stack[pos.stack.size() - 2] = 's';
					// GameBoard[x_new][y_new].stack[1].first = Stand;
			}
		}
	}
}

void Game::make_opponent_move(string s, bool player)
{
	// ADITI TODO
	char first = s.at(0);
	Move m;
	m.x = size - (s.at(2) - '0');
	// printf("%d = size, %c is 2nd char \n", size, s.at(2));
	m.y = s.at(1) - 'a';
	if (first == 'F' || first == 'S' || first == 'C')
	{
		m.place_move = true;
		m.piece = (player == White) ? first : tolower(first);
		makemove(m);
	}
	else
	{
		m.place_move = false;
		m.direction = s.at(3);
		if (s.at(3) == '+')
			m.direction = '-';
		else if (s.at(3) == '-')
			m.direction = '+';
		vector<s_int> drops (s.length() - 4,1);

		for (s_int i = 0; i < s.length() - 4; i++)
			drops[i] = (char)(s.at(i+4) - '0');
		
		m.drops = &drops;
		char x_new = m.x + m.drops->size() * ((m.direction == '+') ? 1 : ((m.direction == '-') ? -1 : 0));
		char y_new = m.y + m.drops->size() * ((m.direction == '>') ? 1 : ((m.direction == '<') ? -1 : 0));
		if (GameBoard[m.x][m.y].top_piece() == 'C')
		{
			Position &p = GameBoard[x_new][y_new];
			if (!p.empty() && p.top_piece() == 'S')
				m.cap_move = true;
		}
		makemove(m);
	}
}

void Game::makemove(Move &m){
	if(m.place_move){
		UpdatePlayer((m.piece < 95), m, false);
		// printf("%d %d piece %c\n", m.x, m.y, m.piece);
		GameBoard[m.x][m.y].stack += m.piece;
		// increment num_black, num_white no need
	}
	else{
		vector<s_int> &d = *m.drops;
		int x_add = (m.direction == '+') ? 1 : ((m.direction == '-') ? -1 : 0);
		int y_add = (m.direction == '>') ? 1 : ((m.direction == '<') ? -1 : 0);
		s_int drop_x = m.x + x_add*(m.drops->size());
		s_int drop_y = m.y + y_add*(m.drops->size());

		UpdatePlayer(GameBoard[m.x][m.y].player(), m, false);
		auto &str = GameBoard[m.x][m.y].stack;

		for(s_int i = d.size() - 1; i>=0; --i){
			GameBoard[drop_x][drop_y].stack += str.substr(str.size() - d[i]);
			str.erase(str.size() - d[i]);

			drop_x -= x_add;
			drop_y -= y_add;
		}
	}
}

void Game::antimove(Move &m){
	if(m.place_move){
		UpdatePlayer((m.piece < 95), m, true);
		GameBoard[m.x][m.y].stack.clear();
		// decrement num_black, num_white
	}
	else{
		string &str = GameBoard[m.x][m.y].stack;
		int y_add = (m.direction == '>') ? 1 : ((m.direction == '<') ? -1 : 0);
		int x_add = (m.direction == '+') ? 1 : ((m.direction == '-') ? -1 : 0);
		s_int x = m.x + x_add;
		s_int y = m.y + y_add;

		UpdatePlayer(GameBoard[m.x + m.drops->size() * x_add][m.y + m.drops->size() * y_add].player(), m, true);
		for(auto &l :*m.drops){
			string &temp = GameBoard[x][y].stack;
			str += temp.substr(temp.size() - l);
			temp.erase(temp.size() - l);

			x += x_add;
			y += y_add;
		}
	}
}

void Game::GetStackable(s_int x, s_int y, bool cap, vector<s_int> &result){
	s_int l = 0;
	s_int r = 0;
	s_int u = 0;
	s_int d = 0;
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

void Game::generate_place_1(Player_Type player, list<Move> &moves)
{
	Player &p = (player == Black) ? p_black : p_white;
	char cap = (player == White) ? 'C' : 'c';
	char flat = (player == White) ? 'F' : 'f';
	for (s_int i = 0; i < size; i++)
	{
		for (s_int j = 0; j < size; j++)
		{
			if (GameBoard[i][j].empty())
			{
				if (p.CapLeft)
					moves.emplace_back(i,j,cap);
				if (p.StonesLeft != 0)
					moves.emplace_back(i,j,flat);
			}
		}
	}
}

void Game::generate_place_2(Player_Type player, list<Move> &moves)
{
	Player &p = (player == Black) ? p_black : p_white;
	char s = (player == White) ? 'S' : 's';
	for (s_int i = 0; i < size; i++)
	{
		for (s_int j = 0; j < size; j++)
		{
			if (GameBoard[i][j].empty())
			{
				if (p.StonesLeft != 0)
					moves.emplace_back(i,j,s);
			}
		}
	}
}

void Game::generate_stack_moves(Player_Type player, list<Move> &moves){
	Player &p = (player == Black) ? p_black : p_white;
	vector<s_int> range(4);
	char dir[4] = {'<', '>', '+', '-'};
	
	if(!p.CapLeft){
		GetStackable(p.x, p.y, true, range);
		s_int shiftmax = min((s_int)size, (s_int)GameBoard[p.x][p.y].stack.size());

		for (s_int i1 = 1; i1 <= shiftmax; ++i1){
			for(s_int r=0; r<4; ++r){
				if(range[r] == -1) continue;
				for(auto &d : AllPerms[i1][range[r]]){
					if(d.back() == 1){
						moves.emplace_back(p.x,p.y,dir[r],&d);
						moves.back().cap_move = true;
					}
				}
			}
		}
	}

	if(p.StonesLeft != 0)
	for(s_int i=0; i<size; ++i){
		for(s_int j=0; j<size; ++j){
			if (!GameBoard[i][j].empty() && GameBoard[i][j].player() == player) {
				s_int shiftmax = min((s_int)size, (s_int)GameBoard[i][j].stack.size()); // max pieces.
 				GetStackable(i,j,false,range);
				for (s_int i1 = 1; i1 <= shiftmax; ++i1){
					for(s_int r=0; r<4; ++r){
						for (s_int m=1; m<=range[r]; ++m){
							for (auto &d : AllPerms[i1][m]){
								moves.emplace_back(i,j,dir[r],&d);
							}
						}
					}
				}
			}
		}
	}
}


eval_type Game::negaMax(bool player,char depth,eval_type alpha,eval_type beta)
{
	eval_type alpha_orig = alpha;
	if (TTable[player].find(to_string()) != TTable[player].end())
	{
		Transposition& t = TTable[player][to_string()];
		if (t.depth >= depth)
		{
			if (t.flag == 'e')
				return t.score;
			else if (t.flag == 'l')
				alpha = max(alpha, t.score);
			else if (t.flag == 'u')
				beta = min(beta, t.score);
			if (alpha >= beta)
				return t.score;
		}
	}
	if (depth == 0 || abs(eval()) > FLWIN)
		return eval();

	eval_type best_val = -2*RDWIN;
	bool done = false;
	vector<list<Move> > opponent_moves (4);
	for (int i = 0; i < 3 && !done; i++)
	{
		switch (i)
		{
			case 0:
				generate_place_1(!player,opponent_moves[i]);
				break;
			case 1:
				generate_place_2(!player,opponent_moves[i]);
				break;
			case 2:
				generate_stack_moves(!player,opponent_moves[i]);
				break;
		}
		for (auto it = opponent_moves[i].begin(); it != opponent_moves[i].end() && !done; it++)
		{
			makemove(*it);
			eval_type child = negaMax(!player,depth-1,-1*beta,-1*alpha);
			best_val = max(best_val, child);
			alpha = max(alpha, child);
			if (alpha >= beta)
				done = true;
			antimove(*it);
		}
	}
	Transposition tt;
	tt.score = best_val;
	if (best_val <= alpha_orig)
		tt.flag = 'u';
	else if (best_val >= beta)
		tt.flag = 'l';
	else
		tt.flag = 'e';
	tt.depth = depth;
	TTable[player][to_string()] = tt;
	return best_val;
}




