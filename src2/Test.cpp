#include "Game.h"
#include "Move.h"

s_int size;

void checkMove()
{
	size = 3;
	Game g(size,21);
	Move m1(2,1,'F');
	g.makemove(m1);
	printf("%s is Board after m1 \n", g.to_string().c_str());

	Move m2(1,1,'s');
	g.makemove(m2);
	printf("%s is Board after m2 \n", g.to_string().c_str());

	Move m3(0,1,'C');
	g.makemove(m3);
	printf("%s is Board after m3 \n", g.to_string().c_str());

	vector<s_int> v (1,1);
	Move m4(0,1,'+', &v);
	m4.cap_move = true;
	g.makemove(m4);
	printf("%s is Board after m4 \n", g.to_string().c_str());

	g.antimove(m4);
	printf("%s is Board after m3 \n", g.to_string().c_str());

	g.antimove(m3);
	printf("%s is Board after m2 \n", g.to_string().c_str());

	g.antimove(m2);
	printf("%s is Board after m1 \n", g.to_string().c_str());
}

void check_makeOppo()
{
	size = 3;
	Game g(size,21);
	Move m1(2,1,'F');
	cout << "Yo, in move \n";
	cout << "Move is " << m1.to_string() << endl;
	g.make_opponent_move(m1.to_string(), White);
	printf("%s is Board after m1 \n", g.to_string().c_str());

	Move m2(1,1,'f');
	cout << "m2 is " << m2.to_string() << endl;
	g.make_opponent_move(m2.to_string(), Black);
	printf("%s is Board after m2 \n", g.to_string().c_str());

	Move m3(0,1,'C');
	g.make_opponent_move(m3.to_string(), White);
	printf("%s is Board after m3 \n", g.to_string().c_str());

	vector<s_int> v (1,1);
	Move m4(0,1,'+', &v);
	// m4.cap_move = true;
	g.make_opponent_move(m4.to_string(), White);
	printf("%s is Board after m4 \n", g.to_string().c_str());

	// g.antimove(m4);
	// printf("%s is Board after m3 \n", g.to_string().c_str());

	// g.antimove(m3);
	// printf("%s is Board after m2 \n", g.to_string().c_str());

	// g.antimove(m2);
	// printf("%s is Board after m1 \n", g.to_string().c_str());
}

void checkValid()
{

}

int main(int argc, char const *argv[])
{
	cout << "Yo \n";
	// checkMove();
	check_makeOppo();
	checkValid();
	return 0;
}