#include "Header.h"

int Size;

void printVec(vector<int> & v){
	for (auto &i : v) std::cout << i;
	std::cout << ",";
}

int main(int argc, char const *argv[])
{
	/* code */
	getAllPerms(5);
	// cout << "ABC! \n";
	// for (int i = 0 ; i <= 5 ; i ++)
	// {
	// 	for (int j = 0 ; j < 5 ; j ++)
	// 	{
	// 		std::cout << i << " " << j << std::endl;
	// 		for (auto &k : AllPerms[i][j])
	// 			printVec(k);
	// 		std::cout << "\n";
	// 	}
	// }
	// cout << "YO! \n";
	Size = 3;
	// bool move = Test::checkMove();
	// if (move) cout << "move, antimove Check karo \n";
	bool valid = Test::checkValid();
	if (valid)
		cout << "valid moves check karo \n";

	return 0;
}