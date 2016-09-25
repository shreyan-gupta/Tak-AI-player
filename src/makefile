CFLAGS = -c -std=c++11 -O3 -march=native
LFLAGS = -std=c++11 -O3 -march=native
OBJ = main.o Board.o Player.o

all : out

out : $(OBJ)
	g++ $(LFLAGS) $(OBJ) -o out

main.o : main.cpp Header.h
	g++ $(CFLAGS) main.cpp

Board.o : Board.cpp Header.h
	g++ $(CFLAGS) Board.cpp

Player.o : Player.cpp Header.h
	g++ $(CFLAGS) Player.cpp

clean : 
	rm *.o
	rm out

run : out
	./out