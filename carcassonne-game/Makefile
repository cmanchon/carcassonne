CC = gcc
CFLAGS = -Wall
OPT = -O3

main: main.o tile.o player.o game.o
	$(CC) $(OPT) obj/main.o obj/tile.o obj/player.o obj/game.o -o main

creative: main-creative.o tile.o player.o game.o
	$(CC) $(OPT) obj/main-creative.o obj/tile.o obj/player.o obj/game.o -o main-creative


main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o obj/main.o

main-creative.o: main-creative.c
	$(CC) -c $(CFLAGS) main-creative.c -o obj/main-creative.o


tile.o: src/tile.c include/tile.h 
	$(CC) -c $(CFLAGS) src/tile.c -o obj/tile.o


player.o: src/player.c include/player.h 
	$(CC) -c $(CFLAGS) src/player.c -o obj/player.o


game.o: src/game.c include/game.h 
	$(CC) -c $(CFLAGS) src/game.c -o obj/game.o



clean: 
	del obj\*.o


cleanux:
	rm obj/*.o


.PHONY: clean