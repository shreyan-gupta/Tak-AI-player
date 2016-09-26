#include "Header.h"

vector<vector<vector<vector<int> > > > AllPerms;

void getAllPerms(int dim)
{
	// fill this matrix!
	// AllPerms[i][j] = all vectors with size = j, sum = i.
	AllPerms = vector<vector<vector<vector<int> > > > (dim+1, std::vector<vector<vector<int> > > (dim));
	for (int sum = 1 ; sum <= dim ; sum ++)
	{
		for (int size = 1 ; size < dim ; size ++)
		{
			vector<vector<int> > & fillthis = AllPerms[sum][size];
			if (size == 1)
			{
				vector<int> x (1,sum);
				fillthis.push_back(x);
			}
			int i = sum-1;
			while (i > 0)
			{
				// pick all vectors at [i][size-1]
				for (auto &j : AllPerms[i][size-1])
				{
					vector<int> x = j;
					x.push_back(sum-i);
					fillthis.push_back(x);
				}
				i --;
			}
		}
	}
}

void printVec(vector<int> & v)
{
	for (auto &i : v)
		std::cout << i;
	std::cout << ",";
}

int main(int argc, char const *argv[])
{
	/* code */
	getAllPerms(5);
	for (int i = 0 ; i <= 5 ; i ++)
	{
		for (int j = 0 ; j < 5 ; j ++)
		{
			std::cout << i << " " << j << std::endl;
			for (auto &k : AllPerms[i][j])
				printVec(k);
			std::cout << "\n";
		}
	}
	return 0;
}