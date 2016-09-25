#include "Header.h"

typedef std::pair<Stone,Player_Type> Piece;
typedef long double ll;

Player::Player(bool player_type, int pieces, int caps){
	Max = player_type;
	StonesLeft = pieces;
	CapsLeft = caps;
}