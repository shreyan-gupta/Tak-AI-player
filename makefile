CC = g++
CFLAGS = -std=c++11 -I include/

TAKOBJECTS = \
	build/tak/bitboard.o \
	build/tak/feature.o \
	build/tak/move.o \
	build/tak/move_generator.o \
	build/tak/util.o

test : build/test

build/test : $(TAKOBJECTS) build/test.o
	$(CC) $^ -o $@

build/%.o : src/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean

clean :
	rm -rf build/