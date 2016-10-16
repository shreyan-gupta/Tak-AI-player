CFLAGS = -c -std=c++11 -Ofast -march=native -g
LFLAGS = -std=c++11 -Ofast -march=native -g
OBJ = Misc.o Game.o Features.o

all : out

out : $(OBJ) main.o
	g++ $(LFLAGS) $(OBJ) main.o -o out

test : $(OBJ) test.o
	g++ $(LFLAGS) $(OBJ) test.o -o test

main.o : main.cpp Game.h
	g++ $(CFLAGS) main.cpp

Game.o : Game.cpp Game.h 
	g++ $(CFLAGS) Game.cpp

Features.o : Features.cpp Game.h
	g++ $(CFLAGS) Features.cpp

Misc.o : Misc.cpp Header.h
	g++ $(CFLAGS) Misc.cpp

Test.o : Test.cpp Game.h
	g++ $(CFLAGS) Test.cpp

Position.hpp : Header.h

Transposition.hpp : Move.hpp Header.h

Move.hpp : Header.h

Game.h : Position.hpp Transposition.hpp

clean : 
	rm *.o
	rm out
	rm test

run : out
	./out

debug : test
	gdb test