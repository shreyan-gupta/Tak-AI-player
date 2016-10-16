CFLAGS = -c -std=c++11 -Ofast -march=native -g
LFLAGS = -std=c++11 -Ofast -march=native -g
OBJ = Misc.o Game.o Features.o

all : out

out : $(OBJ) main.o
	g++ $(LFLAGS) $(OBJ) main.o -o out

test : $(OBJ) Test.o
	g++ $(LFLAGS) $(OBJ) Test.o -o test

main.o : main.cpp Game.h
	g++ $(CFLAGS) main.cpp

Game.o : Game.cpp Game.h 
	g++ $(CFLAGS) Game.cpp

Features.o : Features.cpp Game.h
	g++ $(CFLAGS) Features.cpp

Misc.o : Misc.cpp Move.h
	g++ $(CFLAGS) Misc.cpp

Test.o : Test.cpp Game.h
	g++ $(CFLAGS) Test.cpp

Position.h : Header.h

Transposition.h : Move.h Header.h

Move.h : Header.h

Game.h : Position.h Transposition.h

clean : 
	rm *.o
	rm out
	rm test

run : out
	./out

debug : test
	gdb test