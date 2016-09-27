
#include "Header.h"

bool Test::checkMove()
{
	std::cout << "Moves -> \n";
	Game g(3);

	Piece p = piece(Flat,true);
	Move m(2,1,p);
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

	Piece p = piece(Flat,true);
	Move m(2,1,p);
	cout << m.to_string() << std::endl;
	g.makemove(m);
	std::cout << g.to_string() << std::endl;

	Piece pp = piece(Flat,false);
	Move mm(1,1,pp);
	cout << mm.to_string() << std::endl;
	g.makemove(mm);
	std::cout << g.to_string() << std::endl;
	
	// cout << "MOVES FOR White \n";

	// std::multimap<eval_type, Move> moves;
	// g.generate_valid_moves(White,moves);
	// for (auto &i : moves)
	// 	cout << i.first << " " << i.second.to_string() << endl;

	Move m1(2,1,'-',&AllPerms[1][1][0]);
	cout << m1.to_string() << std::endl;
	g.makemove(m1);
	std::cout << g.to_string() << std::endl;

	cout << "MOVES FOR White \n";

	std::multimap<eval_type, Move> moves1;
	g.generate_valid_moves(White,moves1);
	for (auto &i : moves1)
		cout << i.first << " " << i.second.to_string() << endl;
	return true;
}