#include "Header.h"

Player::Player(bool player_type, int pieces, int caps){
	MinMax = player_type;
	StonesLeft = pieces;
	CapsLeft = caps;
}