
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	string s;
	getline(cin,s);
	cerr << s << "----------- START Msg \n";
	while (true)
	{
		string s1,s2;
		cin >> s1;
		cerr << s1 << " MOVE? \n";
		cin >> s2;
		cout << s2;	
	}
	return 0;
}