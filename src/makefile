CFLAGS = -c -std=c++11 -O3 -march=native -g
LFLAGS = -std=c++11 -O3 -march=native
OBJ = main.o Misc.o Game.o Features.o Test.o

all : out

out : $(OBJ)
	g++ $(LFLAGS) $(OBJ) -o out

main.o : main.cpp Header.h
	g++ $(CFLAGS) main.cpp

Game.o : Game.cpp Header.h
	g++ $(CFLAGS) Game.cpp

Features.o : Features.cpp Header.h
	g++ $(CFLAGS) Features.cpp

Misc.o : Misc.cpp Header.h
	g++ $(CFLAGS) Misc.cpp

Test.o : Test.cpp Header.h
	g++ $(CFLAGS) Test.cpp

clean : 
	rm *.o
	rm out

run : out
	./out