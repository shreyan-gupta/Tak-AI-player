BUILD_TYPE = debug
CC = g++
CFLAGS = -std=c++11 -I include/

ifeq ($(BUILD_TYPE), debug)
	CFLAGS += -g
endif
ifeq ($(BUILD_TYPE), optimize)
	CFLAGS += -O3 -march=native -DNDEBUG
endif

TAKOBJECTS = \
	build/tak/bitboard.o \
	build/tak/feature.o \
	build/tak/move.o \
	build/tak/move_generator.o \
	build/tak/util.o

MINIMAXOBJECTS = \
	build/minimax/minimax.o

all : $(TAKOBJECTS) $(MINIMAXOBJECTS)

test : build/test

build/test : $(TAKOBJECTS) $(MINIMAXOBJECTS) build/test.o
	$(CC) -g $^ -o $@

build/%.o : src/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean

clean :
	rm -rf build/