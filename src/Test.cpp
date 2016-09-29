
#include "Header.h"

bool Test::checkPath(){
	Game g(3);
	multimap<eval_type,Move> moves;
	Move m1(2,0,piece(Flat,White));
	Move m2(2,1,piece(Flat,White));
	// Move m3(2,2,piece(Flat,White));

	g.makemove(m1);
	g.makemove(m2);
	// g.makemove(m3);
	cout << g.to_string();
	g.generate_valid_moves(White,moves);
	for(auto &i : moves)
		cout << i.first << " " << i.second.to_string() << "\n";
}

bool Test::checkMove()
{
	std::cout << "Moves -> \n";
	Game g(3);

	// Piece p = piece(Flat,White);
	Move m(2,1,piece(Flat,White));
	cout << m.to_string() << std::endl;
	g.makemove(m);
	std::cout << g.to_string() << std::endl;

	Piece pp = piece(Flat,false);
	Move mm(1,1,pp);
	cout << mm.to_string() << std::endl;
	g.makemove(mm);
	std::cout << g.to_string() << std::endl;

	Move m1(2,1,'-',&AllPerms[1][1][0]);
	cout << m1.to_string() << std::endl;
	g.makemove(m1);
	std::cout << g.to_string() << std::endl;

	g.antimove(m1);
	std::cout << g.to_string() << std::endl;

	return true;
}

bool Test::checkValid()
{
	std::cout << "Valid Moves -> \n";
	Game g(3);

	Move m(2,1,piece(Cap,White));
	cout << m.to_string() << std::endl;
	g.makemove(m);
	printf("%d : black x, %d : white x \n", g.p_black.x, g.p_white.x);
	std::cout << g.to_string() << std::endl;

	Move mm(1,1,piece(Stand,Black));
	cout << mm.to_string() << std::endl;
	g.makemove(mm);
	printf("%d : black x, %d : white x \n", g.p_black.x, g.p_white.x);
	std::cout << g.to_string() << std::endl;

	printf("Moves for white\n");
	std::multimap<eval_type, Move> moves;
	g.generate_valid_moves(White,moves);
	for (auto &i : moves)
		cout << i.first << " " << i.second.to_string() << endl;
	printf("Done Generating moves, is there a up move(1) ? \n");

	// printf("Top of Stack %d\n", g.GameBoard[2][1].Stack.front().second);
	printf("%d : black x, %d : white x \n", g.p_black.x, g.p_white.x);
	g.antimove(mm);
	printf("%d : black x, %d : white x \n", g.p_black.x, g.p_white.x);

	Move m2(1,1,piece(Cap,Black));
	cout << m2.to_string() << endl;
	g.makemove(m2);
	printf("%d : black x, %d : white x \n", g.p_black.x, g.p_white.x);
	cout << g.to_string() << endl;

	// Move m1(2,1,'-',&AllPerms[1][1][0]);
	// m1.CapMove = true;
	// cout << m1.to_string() << std::endl;
	// g.makemove(m1);
	// std::cout << g.to_string() << std::endl;

	cout << "MOVES FOR White \n";

	std::multimap<eval_type, Move> moves1;
	g.generate_valid_moves(White,moves1);
	for (auto &i : moves1)
		cout << i.first << " " << i.second.to_string() << endl;
	cout << "up (1) nhi hai na?? \n";
	return true;
}