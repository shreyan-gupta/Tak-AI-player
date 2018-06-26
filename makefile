CC = g++
CFLAGS = -std=c++11 -I include/

OBJECTS = \
	build/bitboard.o \
	build/feature.o \
	build/move.o \
	build/move_generator.o \
	build/util.o

all : build/out

test : build/test

build/out : $(OBJECTS) build/main.o
	$(CC) $^ -o $@

build/test : $(OBJECTS) build/test.o
	$(CC) $^ -o $@

build/%.o : src/%.cpp
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean

clean :
	rm -rf build/