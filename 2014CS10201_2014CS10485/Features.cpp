#include "Header.h"
using namespace Types;

inline bool pathable(char x, char y, bool player, vector< vector<Position> > &GameBoard){
	return (!GameBoard[x][y].empty() && GameBoard[x][y].top_piece().first != Stand && GameBoard[x][y].top_piece().second == player);
}

void search(bool type, char x, char y, bool player, vector< vector<Position> > &GameBoard, vector< vector<bool> > &explored, bool &found, char size){
	if(found || explored[x][y] || x<0 || x==size || y<0 || y==size) return;
	if(type && y == size-1){
		found = true;
		return;
	}
	if(!type && x == size-1){
		found = true;
		return;
	}
	explored[x][y] = true;
	if(y!=size-1 && pathable(x,y+1,player,GameBoard))
		search(type, x, y+1, player, GameBoard, explored, found, size);
	if(x!=size-1 && pathable(x+1,y,player,GameBoard)) 
		search(type, x+1, y, player, GameBoard, explored, found, size);
	if(x!=0 && pathable(x-1,y,player,GameBoard)) 
		search(type, x-1, y, player, GameBoard, explored, found, size);
	if(y!=0 && pathable(x,y-1,player,GameBoard)) 
		search(type, x, y-1, player, GameBoard, explored, found, size);
}

// path
eval_type Game::feature0(){
	// fprintf(stderr, "Feature 0 enter \n");
	vector< vector<bool> > explored(size, vector<bool>(size, false));
	for(char i=0; i<size; ++i){
		if(GameBoard[i][0].empty() || explored[i][0]) continue;
		bool found = false;
		char x = i;
		char y = 0;
		if (GameBoard[i][0].top_piece().first != Stand)
			search(true,x,y,GameBoard[i][0].top_piece().second, GameBoard, explored, found, size);
		if(found){
			if(GameBoard[i][0].top_piece().second == White) return w[0];
			else return -w[0];
		}
	}
	explored = vector< vector<bool> >(size, vector<bool>(size, false));
	for(char j=0; j<size; ++j){
		if(GameBoard[0][j].empty() || explored[0][j]) continue;
		bool found = false;
		char x = 0;
		char y = j;
		if (GameBoard[0][j].top_piece().first != Stand)
			search(false,x,y,GameBoard[0][j].top_piece().second, GameBoard, explored, found, size);
		if(found){
			if(GameBoard[0][j].top_piece().second == White) return w[0];
			else return -w[0];
		}
	}
	return 0;
}

// csiochd@123
inline eval_type Game::center(char i, char j)
{
	float half = size/2.0;
	// cout << "Center val is " << (abs(i - half) + abs(j - half))* w[1] << endl;
	return (abs(i - half) + abs(j - half))* w[1];
}


inline eval_type Game::nbr(Piece p1, Piece p2){
	Stone &s1 = p1.first;
	Stone &s2 = p2.first;
	bool same = (p1.second == p2.second);
	int mult = (p1.second == White) ? 1 : -1;
	if(same){
		if(s1 == Flat){
			switch(s2){
				case Flat 	: return mult * w[9];
				case Stand 	: return mult * w[10];
				case Cap	: return mult * w[11];
			}
		}
		else if(s1 == Stand){
			switch(s2){
				case Stand 	: return mult * w[12];
				case Cap 	: return mult * w[13];
				default 	: return 0;
			}
		}
	}
	else{
		if(s1 == Flat){
			switch(s2){
				case Stand 	: return mult * w[14];
				case Cap 	: return mult * w[2];
				default 	: return 0;
			}
		}
		else if(s1 == Stand){
			if(s2 == Cap) return mult * w[3];
		}
		return 0;
	}
}

inline eval_type Game::neighbor(char x, char y)
{
	eval_type ng = 0;
	if(x != size-1 && !GameBoard[x+1][y].empty())
		ng += nbr(GameBoard[x][y].top_piece(), GameBoard[x+1][y].top_piece());

	if(y != size-1 && !GameBoard[x][y+1].empty())
		ng += nbr(GameBoard[x][y].top_piece(), GameBoard[x][y+1].top_piece());

	return ng;
}

inline eval_type Game::stone_weight(Stone s){
	switch(s){
		case Flat : return w[6];
		case Stand : return w[7];
		default : return w[8];
	}
}

inline eval_type Game::top_colors(char x, char y, pair<char,char> &top5)
{
	auto &j = GameBoard[x][y];
	eval_type count = 0;
	eval_type cap_opp = 2.7;
	if(j.top_piece().second == White){
		// count += stone_weight(j.top_piece().first);
		if(j.top_piece().first == Cap)
			count += ((top5.first)*w[4] + (top5.second)*w[5]*cap_opp);
		else 
			count += ((top5.first)*w[4] + (top5.second)*w[5]);
	}
	else{
		// count -= stone_weight(j.top_piece().first);
		if(j.top_piece().first == Cap)
			count -= ((top5.first)*w[5] + (top5.second)*w[4]*cap_opp);
		else 
			count -= ((top5.first)*w[5] + (top5.second)*w[4]);
	}
	return count;
}

inline char Game::sq(char i, char j)
{
	bool yo = true;
	Player_Type p = GameBoard[i][j].top_piece().second;
	if (i + 1 >= size) yo = false;
	else yo = yo & (!GameBoard[i+1][j].empty() && GameBoard[i+1][j].top_piece().second == p);

	if (i - 1 < 0) yo = false;
	else yo = yo & (!GameBoard[i-1][j].empty() && GameBoard[i-1][j].top_piece().second == p);

	if (j + 1 >= size) yo = false;
	else yo = yo & (!GameBoard[i][j+1].empty() && GameBoard[i][j+1].top_piece().second == p);

	if (j - 1 < 0) yo = false;
	else yo = yo & (!GameBoard[i][j-1].empty() && GameBoard[i][j-1].top_piece().second == p);

	if (yo) return 1;
	return 0;
}


// closer to center
eval_type Game::feature1(){
	// fprintf(stderr, "Feature 1 enter \n");
	eval_type count = 0;
	pair<char,char> top5;
	// vector<vector<int> > influence (Size, vector<int> (Size,0));
	bool full = true;
	char flat_count = 0;
	char squares = 0;

	for(char i=0; i<size; ++i){
		for(char j=0; j<size; ++j){
			if(GameBoard[i][j].empty())
			{
				full = false;
				continue;
			}

			eval_type mult = (GameBoard[i][j].top_piece().second == White) ? 1 : -1;
			eval_type stone = stone_weight(GameBoard[i][j].top_piece().first) * w[16];
			GameBoard[i][j].top5(top5);

				// printf("i = %d, j = %d, top5 white = %d, black = %d \n", i, j , top5.first, top5.second);
				// printf("%f Center \n", center(i,j));
				// printf("%f Neighbor\n", neighbor(i,j));
				// printf("%f Top colors \n", top_colors(i,j,top5));


			count += center(i,j)*mult;
			// count += neighbor(i,j); ??
			count += top_colors(i,j,top5);

			count += mult * stone;
			if (i > 0)
				count += mult * stone;
			if (i < size-1)
				count += mult * stone;
			if (j > 0)
				count += mult * stone;
			if (j < size-1)
				count += mult * stone;

				// cout << "Now, count = " << count << endl;

			if (GameBoard[i][j].top_piece().first != Stand && GameBoard[i][j].top_piece().second == White)
				++flat_count;
			else if(GameBoard[i][j].top_piece().first != Stand && GameBoard[i][j].top_piece().second == Black)
				--flat_count;

			squares += mult * sq(i,j);

		}
	}

	if (full)
		count += flat_count*w[15];
	else if ((p_black.StonesLeft + (char)p_black.CapLeft) == 0 || (p_white.StonesLeft + (char)p_white.CapLeft) == 0)
		count += flat_count*w[15];

	count += squares*w[17];

	return count;
}




// neighbours  DONEEEE
// eval_type Game::feature2(){
// 	// fprintf(stderr, "Feature 2 enter \n");
// 	eval_type count = 0;
// 	for(int x=0; x<size; ++x){
// 		for(int y=0; y<size; ++y){
// 			if(GameBoard[x][y].empty()) continue;
// 			if(x != size-1 && !GameBoard[x+1][y].empty())
// 				count += nbr(GameBoard[x][y].top_piece(), GameBoard[x+1][y].top_piece());

// 			if(y != size-1 && !GameBoard[x][y+1].empty())
// 				count += nbr(GameBoard[x][y].top_piece(), GameBoard[x][y+1].top_piece());
// 		}
// 	}
// 	return count;
// }


// How many of the same type below the stack
// Weight of each stack as per piece
// eval_type Game::feature3(){
// 	// fprintf(stderr, "Feature 3 enter \n");
// 	eval_type count = 0;
// 	pair<int,int> top5;
// 	for(auto &i : GameBoard){
// 		for(auto &j : i){
// 			if(j.empty()) continue;
// 			j.top5(top5);
// 			if(j.top_piece().second == White){
// 				// count += stone_weight(j.top_piece().first);
// 				if(j.top_piece().first == Cap)
// 					count += ((top5.first)*w[4] + (top5.second)*w[5]*1);
// 				else 
// 					count += ((top5.first)*w[4] + (top5.second)*w[5]);
// 			}
// 			else{
// 				// count -= stone_weight(j.top_piece().first);
// 				if(j.top_piece().first == Cap)
// 					count -= ((top5.first)*w[4] + (top5.second)*w[5]*1);
// 				else 
// 					count -= ((top5.first)*w[5] + (top5.second)*w[4]);
// 			}
// 		}
// 	}
// 	return count;
// }

// eval_type Game::feature4(){
// 	eval_type count = 0;
// 	bool full = true;
// 	for (int i = 0 ; i < size ; ++i){
// 		for (int j = 0 ; j < size ; ++j){
// 			if (GameBoard[i][j].empty()){
// 				full = false;
// 				continue;
// 			}
// 			if (GameBoard[i][j].top_piece().first != Stand && GameBoard[i][j].top_piece().second == White) ++count;
// 			else if(GameBoard[i][j].top_piece().first != Stand && GameBoard[i][j].top_piece().second == Black) --count;
// 		}
// 	}
// 	if(full) return count*w[15];
// 	else if((p_black.StonesLeft + (int)p_black.CapLeft) == 0 || (p_white.StonesLeft + (int)p_white.CapLeft) == 0)
// 		return count*w[15];
// 	else return 0;
// }


// eval_type Game::feature5(){
// 	vector<vector<int> > influence (Size, vector<int> (Size,0));
// 	int mult = 0;
// 	eval_type count = 0;
// 	for (int i = 0 ; i < Size ; i ++)
// 	{
// 		for (int j = 0 ; j < Size ; j ++)
// 		{
// 			if (GameBoard[i][j].empty()) continue;
// 			mult = (GameBoard[i][j].top_piece().second == White) ? 1 : -1;
// 			mult *= stone_weight(GameBoard[i][j].top_piece().first);
// 			influence[i][j] += mult;
// 			if (i > 0)
// 				influence[i-1][j] += mult;
// 			if (i < size-1)
// 				influence[i+1][j] += mult;
// 			if (j > 0)
// 				influence[i][j-1] += mult;
// 			if (j < size-1)
// 				influence[i][j+1] += mult;
// 		}
// 	}
// 	// counted all!!
// 	for (int i = 0; i < size; i++)
// 	{
// 		for (int j = 0 ; j < size ; j ++)
// 			count += influence[i][j];
// 	}
// 	return count*w[16];
// }

// Number of white and black on board
// eval_type Game::feature4(){
// 	float count = 0;
// 	for(int i=0; i<size; ++i){
// 		for(int j=0; j<size; ++j){
// 			if(GameBoard[i][j].empty()) continue;
// 			count += favourableStack(GameBoard, i, j);
// 		}
// 	}
// 	return count;
// }