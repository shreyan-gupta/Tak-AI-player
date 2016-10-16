#include "Header.h"
using namespace Types;

vector< vector< vector< vector<s_int> > > > AllPerms;

Player::Player(bool player_type, s_int pieces){
	type = player_type;
	StonesLeft = pieces;
	CapLeft = true;
	x = -1;
	y = -1;
}

void getAllPerms(s_int dim)
{
	// fill this matrix!
	// AllPerms[i][j] = all vectors with size = j, sum = i.
	AllPerms = vector<vector<vector<vector<s_int> > > > (dim+1, std::vector<vector<vector<s_int> > > (dim));
	for (s_int sum = 1 ; sum <= dim ; sum ++)
	{
		for (s_int size = 1 ; size < dim ; size ++)
		{
			vector<vector<s_int> > & fillthis = AllPerms[sum][size];
			if (size == 1)
			{
				vector<s_int> x (1,sum);
				fillthis.push_back(x);
			}
			s_int i = sum-1;
			while (i > 0)
			{
				// pick all vectors at [i][size-1]
				for (auto &j : AllPerms[i][size-1])
				{
					vector<s_int> x = j;
					x.push_back(sum-i);
					fillthis.push_back(x);
				}
				i --;
			}
		}
	}
}