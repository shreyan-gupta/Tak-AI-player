#include "Header.h"

// int sum_index[10];
// int count_index[10];
// int max_index[10];

// void Test::print_index(){
// 	FILE *debug;
// 	debug = fopen("debug.txt", "a");
// 	for(int i=0; i<4; ++i){
// 		fprintf(debug, "Depth %d avg %f max %d\n", i, (1.0*sum_index[i]/count_index[i]), max_index[i]);
// 		sum_index[i] = 0;
// 		count_index[i] = 0;
// 		max_index[i] = 0;
// 	}
// 	fclose(debug);
// }

bool Test::checkPath(){
	Game g(3,100);
	multimap<eval_type,Move> moves;
	Move m1(2,0,piece(Flat,White));
	Move m2(2,1,piece(Flat,White));
	// Move m3(2,2,piece(Flat,White));

	g.makemove(m1);
	g.makemove(m2);
	// g.makemove(m3);
	cerr << g.to_string();
	g.generate_valid_moves(White,moves);
	for(auto &i : moves)
		cerr << i.first << " " << i.second.to_string() << "\n";
	return true;
}

bool Test::checkMove()
{
	std::cerr << "Moves -> \n";
	Game g(3,100);

	// Piece p = piece(Flat,White);
	Move m(2,1,piece(Flat,White));
	cerr << m.to_string() << std::endl;
	g.makemove(m);
	std::cerr << g.to_string() << std::endl;

	Piece pp = piece(Flat,false);
	Move mm(1,1,pp);
	cerr << mm.to_string() << std::endl;
	g.makemove(mm);
	std::cerr << g.to_string() << std::endl;

	Move m1(2,1,'-',&AllPerms[1][1][0]);
	cerr << m1.to_string() << std::endl;
	g.makemove(m1);
	std::cerr << g.to_string() << std::endl;

	g.antimove(m1);
	std::cerr << g.to_string() << std::endl;

	return true;
}

bool Test::checkValid()
{
	std::cerr << "Valid Moves -> \n";
	Game g(3,100);

	Move m(2,1,piece(Cap,White));
	cerr << m.to_string() << std::endl;
	g.makemove(m);
	fprintf(stderr, "%d : black x, %d : white x \n", g.p_black.x, g.p_white.x);
	std::cerr << g.to_string() << std::endl;

	Move mm(1,1,piece(Stand,Black));
	cerr << mm.to_string() << std::endl;
	g.makemove(mm);
	fprintf(stderr, "%d : black x, %d : white x \n", g.p_black.x, g.p_white.x);
	std::cerr << g.to_string() << std::endl;

	fprintf(stderr, "Moves for white\n");
	std::multimap<eval_type, Move> moves;
	g.generate_valid_moves(White,moves);
	for (auto &i : moves)
		cerr << i.first << " " << i.second.to_string() << endl;
	fprintf(stderr, "Done Generating moves, is there a up move(1) ? \n");

	// fprintf(stderr, "Top of Stack %d\n", g.GameBoard[2][1].Stack.front().second);
	// fprintf(stderr, "%d : black x, %d : white x \n", g.p_black.x, g.p_white.x);
	g.antimove(mm);
	// fprintf(stderr, "%d : black x, %d : white x \n", g.p_black.x, g.p_white.x);

	Move m2(1,1,piece(Cap,Black));
	cerr << m2.to_string() << endl;
	g.makemove(m2);
	fprintf(stderr, "%d : black x, %d : white x \n", g.p_black.x, g.p_white.x);
	cerr << g.to_string() << endl;

	// Move m1(2,1,'-',&AllPerms[1][1][0]);
	// m1.CapMove = true;
	// cerr << m1.to_string() << std::endl;
	// g.makemove(m1);
	// std::cerr << g.to_string() << std::endl;

	cerr << "MOVES FOR White \n";

	std::multimap<eval_type, Move> moves1;
	g.generate_valid_moves(White,moves1);
	for (auto &i : moves1)
		cerr << i.first << " " << i.second.to_string() << endl;
	cerr << "up (1) nhi hai na?? \n";
	return true;
}


bool Test::checkfavourable()
{
	Game g(3,100);

	Move m(2,1,piece(Flat,White));
	cerr << m.to_string() << std::endl;
	g.makemove(m);
	fprintf(stderr, "%d : black x, %d : white x \n", g.p_black.x, g.p_white.x);
	std::cerr << g.to_string() << std::endl;

	Move mm(1,1,piece(Cap,Black));
	cerr << mm.to_string() << std::endl;
	g.makemove(mm);
	fprintf(stderr, "%d : black x, %d : white x \n", g.p_black.x, g.p_white.x);
	std::cerr << g.to_string() << std::endl;

	// cerr << favourableStack(g.GameBoard, 2,1) << endl;
	return true;
}