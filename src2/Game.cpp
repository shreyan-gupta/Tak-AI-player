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
		{
			for (int i = 0; i < (m.drops)->size(); i++)
				cout << (int)((*m.drops)[i]) << " ";
			cout << "..... This is the drops array \n";
		}
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

// void Game::generate_place_1(Player_Type player, list<Move> &moves)
// {
// 	Player &p = (player == Black) ? p_black : p_white;
// 	char cap = (player == White) ? 'C' : 'c';
// 	char flat = (player == White) ? 'F' : 'f';
// 	for (s_int i = 0; i < size; i++)
// 	{
// 		for (s_int j = 0; j < size; j++)
// 		{
// 			if (GameBoard[i][j].empty())
// 			{
// 				if (p.CapLeft)
// 					moves.emplace_back(i,j,cap);
// 				if (p.StonesLeft != 0)
// 					moves.emplace_back(i,j,flat);
// 			}
// 		}
// 	}
// }

// void Game::generate_place_2(Player_Type player, list<Move> &moves)
// {
// 	Player &p = (player == Black) ? p_black : p_white;
// 	char s = (player == White) ? 'S' : 's';
// 	for (s_int i = 0; i < size; i++)
// 	{
// 		for (s_int j = 0; j < size; j++)
// 		{
// 			if (GameBoard[i][j].empty())
// 			{
// 				if (p.StonesLeft != 0)
// 					moves.emplace_back(i,j,s);
// 			}
// 		}
// 	}
// }

// void Game::generate_stack_moves(Player_Type player, list<Move> &moves){
// 	Player &p = (player == Black) ? p_black : p_white;
// 	vector<s_int> range(4);
// 	char dir[4] = {'<', '>', '+', '-'};
	
// 	if(!p.CapLeft){
// 		GetStackable(p.x, p.y, true, range);
// 		s_int shiftmax = min((s_int)size, (s_int)GameBoard[p.x][p.y].stack.size());

// 		for (s_int i1 = 1; i1 <= shiftmax; ++i1){
// 			for(s_int r=0; r<4; ++r){
// 				if(range[r] == -1) continue;
// 				for(auto &d : AllPerms[i1][range[r]]){
// 					if(d.back() == 1){
// 						moves.emplace_back(p.x,p.y,dir[r],&d);
// 						moves.back().cap_move = true;
// 					}
// 				}
// 			}
// 		}
// 	}

// 	if(p.StonesLeft != 0)
// 	for(s_int i=0; i<size; ++i){
// 		for(s_int j=0; j<size; ++j){
// 			if (!GameBoard[i][j].empty() && GameBoard[i][j].player() == player) {
// 				s_int shiftmax = min((s_int)size, (s_int)GameBoard[i][j].stack.size()); // max pieces.
//  				GetStackable(i,j,false,range);
// 				for (s_int i1 = 1; i1 <= shiftmax; ++i1){
// 					for(s_int r=0; r<4; ++r){
// 						for (s_int m=1; m<=range[r]; ++m){
// 							for (auto &d : AllPerms[i1][m]){
// 								moves.emplace_back(i,j,dir[r],&d);
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// }


void Game::generate_valid_moves(Player_Type player, multimap<pair<eval_type,eval_type>,Move> &moves)
{
	Player &p = (player == Black) ? p_black : p_white;


	// PLACE 1
	char cap = (player == White) ? 'C' : 'c';
	char flat = (player == White) ? 'F' : 'f';
	for (s_int i = 0; i < size; i++)
	{
		for (s_int j = 0; j < size; j++)
		{
			if (GameBoard[i][j].empty())
			{
				if (p.CapLeft)
				{
					Move m (i,j,cap);
					makemove(m);
					Transposition &t = getTransposition(!player);
					moves.emplace(make_pair(-t.depth + t.score/500,t.score),m);
					// search in tt
					antimove(m);
				}
				if (p.StonesLeft != 0)
				{
					Move m(i,j,flat);
					makemove(m);
					Transposition &t = getTransposition(!player);
					moves.emplace(make_pair(-t.depth + t.score/500,t.score),m);
					// search in tt
					antimove(m);

				}
			}
		}
	}

	// PLACE 2
	char s = (player == White) ? 'S' : 's';
	for (s_int i = 0; i < size; i++)
	{
		for (s_int j = 0; j < size; j++)
		{
			if (GameBoard[i][j].empty())
			{
				if (p.StonesLeft != 0)
				{
					Move m(i,j,s);
					makemove(m);
					Transposition &t = getTransposition(!player);
					moves.emplace(make_pair(-t.depth + t.score/500,t.score),m);
					// search in tt
					antimove(m);
				}
			}
		}
	}

	// STACK
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
						Move m(p.x,p.y,dir[r],&d);
						m.cap_move = true;
						makemove(m);
						Transposition &t = getTransposition(!player);
						moves.emplace(make_pair(-t.depth + t.score/500,t.score),m);
						// search in tt
						antimove(m);
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
								Move m(i,j,dir[r],&d);
								makemove(m);
								Transposition &t = getTransposition(!player);
								moves.emplace(make_pair(-t.depth + t.score/500,t.score),m);
								// search in tt
								antimove(m);
							}
						}
					}
				}
			}
		}
	}
};

string tab(int n){
	string s;
	for(int i=0; i<n; ++i) s += "\t";
	return s;
}

inline void update_trans(Transposition &t, int depth, eval_type best_val, Move *best_move, eval_type alpha_orig, eval_type beta){
	t.score = best_val;
	if (best_val <= alpha_orig)	t.flag = 'u';
	else if (best_val >= beta) t.flag = 'l';
	else t.flag = 'e';
	t.depth = depth;
	t.best_move = *best_move;

	if (t.best_move.x == -1 || t.depth <= 0)
	{
		cerr << t.best_move.x << ", depth = " << t.depth << endl;
		cerr << "ERRORRRRRRR INVALID MOVE! \n";
		int c;
		cin >> c;
	}
}

eval_type Game::negaMax(bool player, s_int depth, eval_type alpha, eval_type beta, pair<Move, Move> &killer)
{
	eval_type alpha_orig = alpha;
	assert (depth >= 0);
	Transposition &t = getTransposition(player);

	if(depth == 0 || abs(t.score) > FLWIN / 2){
		return t.score;
	}

	if (t.depth >= depth)
	{
		if (t.flag == 'e') return t.score;
		else if (t.flag == 'l')	alpha = max(alpha, t.score);
		else if (t.flag == 'u') beta = min(beta, t.score);
		if (alpha >= beta) return t.score;
	}
	else{
		pair<Move, Move> useless_stuff;
		negaMax(player, depth-1, alpha, beta, useless_stuff);
	}

	eval_type best_val = -2*FLWIN;
	Move *best_move = NULL;
	int count = 0;
	pair<Move, Move> next_killer;

	// PV move
	if (t.depth > 0)
	{
		assert(t.best_move.x != -1);
		makemove(t.best_move);
		best_val = -negaMax(!player,depth-1,-beta,-alpha, next_killer);
		antimove(t.best_move);
		best_move = &t.best_move;
		
		alpha = max(alpha, best_val);
		if (alpha >= beta || best_val > FLWIN / 2){
			update_trans(t, depth, best_val, best_move, alpha_orig, beta);
			// cerr << "pruned at PV!! depth = " << depth << endl;
			return best_val;
		}
	}

	// Killer move
	if(isMoveValid(killer.first, player)){
		makemove(killer.first);
		best_val = -negaMax(!player,depth-1,-beta,-alpha, next_killer);
		antimove(killer.first);
		best_move = &killer.first;
		
		alpha = max(alpha, best_val);
		if (alpha >= beta || best_val > FLWIN / 2){
			update_trans(t, depth, best_val, best_move, alpha_orig, beta);
			// cerr << "pruned at killer move 1 !!!! depth = " << depth << endl;
			return best_val;
		}
	}
	if(isMoveValid(killer.second, player)){
		makemove(killer.second);
		best_val = -negaMax(!player,depth-1,-beta,-alpha, next_killer);
		antimove(killer.second);
		best_move = &killer.second;
		
		alpha = max(alpha, best_val);
		if (alpha >= beta || best_val > FLWIN / 2){
			update_trans(t, depth, best_val, best_move, alpha_orig, beta);
			// cerr << "pruned at killer move 2 !!!! depth = " << depth << endl;
			Move temp = killer.first;
			killer.first = killer.second;
			killer.second = temp;
			return best_val;
		}
	}

	multimap< pair<eval_type, eval_type>, Move> move_list;
	generate_valid_moves(player, move_list);

	if(depth == 1){
		best_val = -move_list.begin()->first.second;
		best_move = &(move_list.begin()->second);
		update_trans(t, depth, best_val, best_move, alpha_orig, beta);
		return best_val;
	}

			// if(depth == 2)
			// for(auto itr = move_list.begin(); itr != move_list.end(); ++itr){
			// 	fprintf(stderr, "%d %f : %s\n", itr->first.first, itr->first.second, itr->second.to_string().c_str());
			// }
	// fprintf(stderr, "%f %f\n*********\n", alpha, beta);

	// if(move_list.begin()->second.to_string().compare(t.best_move.to_string()) != 0 && t.depth != 0){
		// fprintf(stderr, "Error! %d %d %d %f %s %f %s\n", depth, move_list.begin()->first.first, t.depth, move_list.begin()->first.second, move_list.begin()->second.to_string().c_str(), t.score, t.best_move.to_string().c_str());
	// }

	// int best_move_index = 0;
	bool window_solution_found = false;
	eval_type window_val = -2*FLWIN;
	Move *window_move = best_move;
	bool prune = false;
	int total_count = 0;
	for(auto itr = move_list.begin(); itr != move_list.end() /*&& count <= 5*/; ++itr)
	{
		makemove(itr->second);
		eval_type child = -negaMax(!player,depth-1,-beta,-alpha, next_killer);
		antimove(itr->second);
			if(!window_solution_found) ++count;
			if(child < -FLWIN / 2 && !window_solution_found) count = 0;
			if(count > 10) window_solution_found = true;
		++total_count;

		if(child > best_val){
			best_val = child;
			best_move = &(itr->second);
			// best_move_index = max(best_move_index, total_count);

			if(!window_solution_found){
				count = 0;
				window_val = child;
				window_move = &(itr->second);
			}
		}
		alpha = max(alpha, child);
		if (alpha >= beta || (child) > FLWIN / 2){
			prune = true;
			break;
		}
		if (depth == 2 && window_solution_found)
			break;
	}
	killer.second = killer.first;
	killer.first = *best_move;
	
	assert(window_move != NULL);
	if(depth >= 2 && window_val != -2*FLWIN && window_val!=best_val){
		fprintf(stderr, "%sD %d TC %d Better %d WE %f AE %f Diff %f prune %d WM %s AM %s\n", tab(depth).c_str(), depth, total_count, (window_val!=best_val), window_val, best_val, (best_val - window_val), prune, window_move->to_string().c_str(), best_move->to_string().c_str());
	}
	update_trans(t, depth, best_val, best_move, alpha_orig, beta);
	return best_val;
}

void Game::print_move_seq(int depth){
	vector<Move> move_list;
	bool player = !opponent_type;
	for(int i=0; i<depth; ++i){
		auto &t = getTransposition(player);
		move_list.push_back(t.best_move);
		if(i==0) cerr << t.score << "\t" << depth << " ";
			if (t.best_move.x == -1 )
			{
				cerr << "ERRORRRRRRR INVALID MOVE! \n";
				break;
				// int c;
				// cin >> c;
			}
		cerr << t.best_move.to_string() << " ";
		player = !player;
		makemove(t.best_move);
	}
	cerr << endl;
	for(auto it = move_list.rbegin(); it != move_list.rend(); ++it){
		antimove(*it);
	}
}

string Game::ids(int depth){
	// int depth = 4;
	cout << to_string() << endl;
	cout << depth << " is the depth \n";
	for(int d=1; d<=depth; ++d){
		pair<Move, Move> useless_stuff;
		eval_type val =  negaMax(!opponent_type, d, -2*RDWIN, 2*RDWIN, useless_stuff);
		print_move_seq(d);
	}
	Transposition& t = getTransposition(!opponent_type);
	
	makemove(t.best_move);
	return t.best_move.to_string();
}

int Game::decide_Depth()
{
	s_int used_black = pieces - p_black.StonesLeft;
	s_int used_white  = pieces - p_white.StonesLeft;
	if (used_black < 3 || used_white < 3)
		return 4;
	if (used_black < 4 || used_white < 4)
		return 4;
	else if (used_white > pieces - 7 || used_black > pieces - 7)
		return 4;
	else return 5;
	// initial 4
	// middle 5
	// before end tk 5
	// ekdum end 4
}

bool Game::isMoveValid(Move &m, bool x)
{
	if (m.x == -1) return false;
	if (m.place_move)
	{
		// x,y should be empty!!
		return GameBoard[m.x][m.y].empty();
	}
	else
	{
		auto &p = GameBoard[m.x][m.y];

		bool valid = (!p.empty() && x == p.player());
		valid = valid && ( (m.cap_move) ? ((p.top_piece()) == 'C') : true );
		int x = (m.direction == '+') ? 1 : ((m.direction == '-') ? -1 : 0);
		int y = (m.direction == '>') ? 1 : ((m.direction == '<') ? -1 : 0);

		s_int dropx = m.x + x*(m.drops->size());
		s_int dropy = m.y + y*(m.drops->size());
		vector<s_int> &d = *m.drops;
		s_int total = 0;
		s_int i = 0;
		for (i; i < d.size()-1; i++)
		{
			valid = valid && (GameBoard[dropx][dropy].empty() || (GameBoard[dropx][dropy].top_piece()) == 'F');
			dropx -= x;
			dropy -= y;
			total += d[i];
		}
		total += d[i];

		bool last_stand = (!GameBoard[dropx-x][dropy-y].empty()) && (GameBoard[dropx-x][dropy-y].top_piece() == 'S');
		bool last_flat = GameBoard[dropx-x][dropy-y].empty() || ((GameBoard[dropx-x][dropy-y].top_piece() == 'F'));
		valid = valid && ((m.cap_move) ? last_stand : last_flat);
		valid = valid && (p.stack.length() >= total);

		return valid;
	}
}
