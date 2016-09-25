#include "Header.h"

typedef std::pair<Stone,Player_Type> Piece;
typedef long double eval_type;

Player::Player(bool player_type, int pieces, int caps){
	Max = player_type;
	StonesLeft = pieces;
	CapsLeft = caps;
}