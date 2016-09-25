#include "Header.h"

typedef std::pair<Stone,Player_Type> Piece;
typedef long double ll;

Player::Player(bool player_type, int pieces, int caps){
	Max = player_type;
	StonesLeft = pieces;
	CapsLeft = caps;
}

Player::~Player()
{
	 // do nothing
}

std::pair<Move,ll> Player::decide_move(Board b, int depth)
{
	std::multimap<ll,Move> allmoves;
	b.generate_valid_moves(Max,allmoves);
	// iterate over allmoves.
	for (auto &i : allmoves)
	{
		// makemove.
		// ask p2 to decide his best move
		// make anti move.
	}
}