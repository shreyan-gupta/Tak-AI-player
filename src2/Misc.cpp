vector< vector< vector< vector<char> > > > AllPerms;

void getAllPerms(char dim)
{
	// fill this matrix!
	// AllPerms[i][j] = all vectors with size = j, sum = i.
	AllPerms = vector<vector<vector<vector<char> > > > (dim+1, std::vector<vector<vector<char> > > (dim));
	for (char sum = 1 ; sum <= dim ; sum ++)
	{
		for (char size = 1 ; size < dim ; size ++)
		{
			vector<vector<char> > & fillthis = AllPerms[sum][size];
			if (size == 1)
			{
				vector<char> x (1,sum);
				fillthis.push_back(x);
			}
			char i = sum-1;
			while (i > 0)
			{
				// pick all vectors at [i][size-1]
				for (auto &j : AllPerms[i][size-1])
				{
					vector<char> x = j;
					x.push_back(sum-i);
					fillthis.push_back(x);
				}
				i --;
			}
		}
	}
}