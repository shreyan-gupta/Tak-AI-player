#include "Game.h"

// const eval_type FLAT_MUL	  = 1.3;
s_int WINDOW;
vector<eval_type> GROUP;
const eval_type RDWIN 		  = 1000000;
const eval_type FLWIN 		  = 1000000;
const eval_type ENDGAMEFLAT	  = 1400; 			// Last best working 1000   Originals : 800 
const eval_type TOPFLAT		  = 500;			// Last best working 450   Originals : 400 
const eval_type STAND		  = 200; 			// Last best working 200   Originals : 200 
const eval_type CAP 		  = 300; 			// Last best working 300   Originals : 300 
const eval_type HARD_FCAPTIVE = 200;			// Last best working 200   Originals : 200 
const eval_type SOFT_FCAPTIVE = -250;			// Last best working -250  Originals : -200
const eval_type HARD_SCAPTIVE = 300; /*TOPFLAT  - 25;*/	// Last best working 300   Originals : 300 
const eval_type SOFT_SCAPTIVE = -150; /*-TOPFLAT - 50;*/	// Last best working -150  Originals : -150
const eval_type HARD_CCAPTIVE = 250; /*TOPFLAT  - 25;*/	// Last best working 250   Originals : 250 
const eval_type SOFT_CCAPTIVE = -150; /*-TOPFLAT - 50;*/	// Last best working -150  Originals : -150

const eval_type CENTER 		  = 7;
const eval_type ENDGAMECUTOFF = 7;

vector<vector<time_t> > time_moves;
vector<int> depth_moves;
vector<time_t> avg_time;
int moves;
time_t cutoff_time;

Game::Game(s_int s, s_int pieces) : p_white(Player(White, pieces)), p_black(Player(Black, pieces)){
	this->size = s;
	GameBoard = vector< vector<Position> >(size, vector<Position>(size));
	TTable = vector<unordered_map<string,Transposition> >(2);
	GROUP = vector<eval_type>(7,0);
	GROUP[2] = 30;
	GROUP[3] = 100;
	GROUP[4] = 300;
	GROUP[5] = 500;
	GROUP[size-1] = RDWIN;
	switch(size){
		case 5 : WINDOW = 127; break;
		case 6 : WINDOW = 20; break;
		case 7 : WINDOW = 20; break;
	}
	time_moves = vector< vector<time_t> > (6,vector<time_t> (5,0));
	depth_moves = vector<int> (6,0);
	avg_time = vector<time_t> (6,0);
	max_time_depth = vector<time_t> (6,0);
	max_time_depth[2] = 2;
	max_time_depth[3] = 5;
	max_time_depth[4] = (size == 7) ? 13 : 9;
	max_time_depth[5] = 14; 
	moves = 0;
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
	char first = s.at(0);
	Move m;
	m.x = size - (s.at(2) - '0');
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


void Game::generate_valid_moves(Player_Type player, multimap<eval_type, Move> &moves)
{
	Player &p = (player == Black) ? p_black : p_white;
	string b = to_string();

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
					moves.emplace(t.score, m);
					// moves.emplace(make_pair(-t.depth + 501*t.score/500,t.score),m);
					// moves.emplace(make_pair(HistoryTable[player][m.to_string()] + t.score,t.score),m);
					// search in tt
					antimove(m);
				}
				if (p.StonesLeft != 0)
				{
					Move m(i,j,flat);
					makemove(m);
					Transposition &t = getTransposition(!player);
					moves.emplace(t.score, m);
					// moves.emplace(make_pair(-t.depth + 501*t.score/500,t.score),m);
					// moves.emplace(make_pair(HistoryTable[player][m.to_string()] + t.score,t.score),m);
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
					moves.emplace(t.score, m);
					// moves.emplace(make_pair(-t.depth + 501*t.score/500,t.score),m);
					// moves.emplace(make_pair(HistoryTable[player][m.to_string()] + t.score,t.score),m);
					// search in tt
					antimove(m);
					// assert(to_string().compare(b) == 0);
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
						moves.emplace(t.score, m);
						// moves.emplace(make_pair(-t.depth + 501*t.score/500,t.score),m);
						// moves.emplace(make_pair(HistoryTable[player][m.to_string()] + t.score,t.score),m);
						// search in tt
						antimove(m);
						// assert(to_string().compare(b) == 0);
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
								moves.emplace(t.score, m);
								// moves.emplace(make_pair(-t.depth + 501*t.score/500,t.score),m);
								// moves.emplace(make_pair(HistoryTable[player][m.to_string()] + t.score,t.score),m);
								// search in tt
								antimove(m);
								// assert(to_string().compare(b) == 0);
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

pair<eval_type, bool> Game::negaMax(bool player, s_int depth, eval_type alpha, eval_type beta, pair<Move, Move> &killer)
{
	// assert (depth >= 0);
	if(time(0) > cutoff_time) return make_pair(0,false);
	eval_type alpha_orig = alpha;
	Transposition &t = getTransposition(player);

	if(depth == 0 || abs(t.score) > FLWIN / 2){
		return make_pair(t.score,true);
	}

	if (t.depth >= depth)
	{
		if (t.flag == 'e') return t.score;
		else if (t.flag == 'l')	alpha = max(alpha, t.score);
		else if (t.flag == 'u') beta = min(beta, t.score);
		if (alpha >= beta) return make_pair(t.score,true);
	}
	else{
		pair<Move, Move> useless_stuff;
		negaMax(player, depth-1, alpha, beta, useless_stuff);
	}

	eval_type best_val = -2*FLWIN;
	Move *best_move = NULL;
	int count = 0;
	pair<Move, Move> next_killer;
	bool done = false;
	bool broken = false;

	// assert(to_string().compare(b) == 0);

	// PV move
	if (t.depth > 0)
	{
		// assert(t.best_move.x != -1);
		makemove(t.best_move);
		auto child = negaMax(!player,depth-1,-beta,-alpha, next_killer);
		antimove(t.best_move);
		if(!child.second) return child;
		else best_val = -child.first;
		best_move = &t.best_move;
		
		alpha = max(alpha, best_val);
		if (alpha >= beta || best_val > FLWIN / 2){
			update_trans(player,t, depth, best_val, best_move, alpha_orig, beta, false);
			// cerr << "pruned at PV!! depth = " << depth << endl;
			return make_pair(best_val,true);
		}
	}

	// assert(to_string().compare(b) == 0);

	// Killer move
	if(isMoveValid(killer.first, player)){	
		makemove(killer.first);
		auto child = negaMax(!player,depth-1,-beta,-alpha, next_killer);
		antimove(killer.first);
		if(!child.second) return child;
		else best_val = -child.first;
		best_move = &killer.first;
		
		alpha = max(alpha, best_val);
		if (alpha >= beta || best_val > FLWIN / 2){
			update_trans(player, t, depth, best_val, best_move, alpha_orig, beta, false);
			// cerr << "pruned at killer move 1 !!!! depth = " << depth << endl;
			return make_pair(best_val,true);
		}
	}
	if(isMoveValid(killer.second, player)){
		makemove(killer.second);
		auto child = negaMax(!player,depth-1,-beta,-alpha, next_killer);
		antimove(killer.second);
		if(!child.second) return child;
		else best_val = -child.first;
		best_move = &killer.second;

		alpha = max(alpha, best_val);
		if (alpha >= beta || best_val > FLWIN / 2){
			update_trans(player, t, depth, best_val, best_move, alpha_orig, beta, false);
			// cerr << "pruned at killer move 2 !!!! depth = " << depth << endl;
			Move temp = killer.first;
			killer.first = killer.second;
			killer.second = temp;
			return make_pair(best_val,true);
		}
	}

	multimap<eval_type, Move> move_list;
	generate_valid_moves(player, move_list);

	if(depth == 1){
		best_val = -move_list.begin()->first;
		best_move = &(move_list.begin()->second);
		update_trans(player, t, depth, best_val, best_move, alpha_orig, beta, false);
		return make_pair(best_val,true);
	}

	// bool window_solution_found = false;
	// eval_type window_val = best_val;
	// Move *window_move = best_move;

	// bool prune = false;
	// int total_count = 0;
	// int window_index;
	// int best_index;


	for(auto itr = move_list.begin(); itr != move_list.end(); ++itr)
	{
		++count;
		makemove(itr->second);
		auto child = negaMax(!player,depth-1,-beta,-alpha,next_killer);
		antimove(itr->second);
		if(!child.second){
			broken = true;
			break;
		}
		if(-child.first < -FLWIN/2) count = 0;
		if(-child.first > best_val){
			count = 0;
			best_val = -child.first;
			best_move = &(itr->second);
		}
		alpha = max(alpha, -child.first);
		if (alpha >= beta || -child.first > FLWIN/2) break;
		if (depth == 2 && count > WINDOW) break;
	}

	update_trans(player, t, depth, best_val, best_move, alpha_orig, beta, broken);
	killer.second = killer.first;
	killer.first = *best_move;
	return make_pair(best_val,true);
}

void Game::print_move_seq(int depth){
	vector<Move> move_list;
	bool player = !opponent_type;
	for(int i=0; i<depth; ++i){
		auto &t = getTransposition(player);
		if (t.best_move.x == -1) break;
		move_list.push_back(t.best_move);
		if(i==0) cerr << t.score << "\t" << depth << " ";
		cerr << t.best_move.to_string() << " ";
		player = !player;
		makemove(t.best_move);
	}
	cerr << endl;
	for(auto it = move_list.rbegin(); it != move_list.rend(); ++it){
		antimove(*it);
	}
}

string Game::ids(){
	// int depth = 4;
	cerr << to_string() << endl;
	// find cut off time -> 
	s_int used_black = pieces - p_black.StonesLeft;
	s_int used_white = pieces - p_white.StonesLeft;
	int depth = 4;
	cutoff_time = time(0) + (time_t)TimeLimit*1000;
	if (!(used_black < 3 || used_white < 3))
	{
		s_int depth = ((size > 5) ? 4 : 5);
		s_int empty_squares = 0;
		time_t time_rem = start_time + (time_t)TimeLimit*1000 - time(0);
		for(auto &i : GameBoard)
			for(auto &j : i)
				if(j.empty()) ++empty_squares;
		while (depth > 1 && avg_time[depth] > time_rem/(10 + empty_squares))
			depth--;
		cutoff_time = time(0) + min(max_time_depth[depth], time_rem/2); // TODO
	}
	auto start_time = time(0);
	for(int d=1; d<=depth; ++d){
		pair<Move, Move> useless_stuff;
		auto val =  negaMax(!opponent_type, d, -2*RDWIN, 2*RDWIN, useless_stuff);
		print_move_seq(d);
		if (!val.second)
		{
			d = d-1;
			cerr << "Actual depth = " << d << endl;
			auto time_taken = time(0) - start_time;
			avg_time[d] = (avg_time[d]*5 - times_moves[d][depth_moves[d]] + time_taken)/5;
			times_moves[d][depth_moves[d]] = time_taken;
			depth_moves[d] += 1;
		}
	}
	Transposition& t = getTransposition(!opponent_type);
	makemove(t.best_move);
	moves += 1;
	return t.best_move.to_string();
}

// int Game::decide_Depth()
// {
// 	s_int used_black = pieces - p_black.StonesLeft;
// 	s_int used_white  = pieces - p_white.StonesLeft;
// 	if (used_black < 3 || used_white < 3 || size >= 6)
// 		return 4;
// 	if (used_black < 4 || used_white < 4)
// 		return 4;
// 	else if (used_white > pieces - 7 || used_black > pieces - 7)
// 		return 4;
// 	else return 5;
// 	// initial 4
// 	// middle 5
// 	// before end tk 5
// 	// ekdum end 4
// }

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

		s_int dropx = m.x;
		s_int dropy = m.y;
		vector<s_int> &d = *m.drops;
		s_int total = 0;
		s_int i = 0;
		for (; i < d.size()-1; i++)
		{
			dropx += x;
			dropy += y;
			valid = valid && (GameBoard[dropx][dropy].empty() || ((GameBoard[dropx][dropy].top_piece()) == 'F'));
			total += d[i];
		}
		total += d[i];

		bool last_stand = (!GameBoard[dropx+x][dropy+y].empty()) && (GameBoard[dropx+x][dropy+y].top_piece() == 'S');
		bool last_flat = GameBoard[dropx+x][dropy+y].empty() || ((GameBoard[dropx+x][dropy+y].top_piece() == 'F'));

		valid = valid && ((m.cap_move) ? last_stand : last_flat);
		valid = valid && (p.stack.length() >= total);

		return valid;
	}
}
