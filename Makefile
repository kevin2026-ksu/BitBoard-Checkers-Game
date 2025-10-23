CC=gcc
CFLAGS=-O2 -std=c11 -Wall -Wextra -Wno-unused-parameter

OBJS=main.o bitboard.o game.o ui.o save.o

all: checkers

checkers: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c game.h ui.h save.h bitboard.h
bitboard.o: bitboard.c bitboard.h
game.o: game.c game.h bitboard.h
ui.o: ui.c ui.h game.h bitboard.h
save.o: save.c save.h game.h bitboard.h

clean:
	rm -f $(OBJS) checkers
