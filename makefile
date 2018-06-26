test:
	g++ -std=c++11 -c src/bitboard.cpp -I include -o build/bitboard.o
	g++ -std=c++11 -c src/move.cpp -I include -o build/move.o
	g++ -std=c++11 -c src/test.cpp -I include -o build/test.o
	g++ -std=c++11 -c src/util.cpp -I include -o build/util.o
	g++ -std=c++11 build/test.o build/bitboard.o build/move.o build/util.o -o build/out
	build/out