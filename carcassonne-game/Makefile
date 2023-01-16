CC = gcc
CFLAGS = -Wall
OPT = -O3

main: main.o tile.o player.o
	$(CC) $(OPT) obj/main.o obj/tile.o obj/player.o -o main


main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o obj/main.o


tile.o: src/tile.c include/tile.h 
	$(CC) -c $(CFLAGS) src/tile.c -o obj/tile.o


player.o: src/player.c include/player.h 
	$(CC) -c $(CFLAGS) src/player.c -o obj/player.o



clean: 
	del obj\*.o


cleanux:
	rm obj/*.o


.PHONY: clean