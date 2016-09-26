CFLAGS = -c -std=c++11 -O3 -march=native
LFLAGS = -std=c++11 -O3 -march=native
OBJ = main.o Misc.o Game.o Features.o

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

clean : 
	rm *.o
	rm out

run : out
	./out