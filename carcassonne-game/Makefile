CC = gcc
CFLAGS = -Wall
OPT = -O3
OBJ = obj/tile.o obj/player.o obj/game.o obj/ai.o

main: obj/main.o $(OBJ)
	$(CC) $(OPT) $< $(OBJ) -o $@

creative: obj/main-creative.o $(OBJ)
	$(CC) $(OPT) $< $(OBJ) -o $@

obj/main.o: main.c
	$(CC) -c $(CFLAGS) $< -o $@

obj/main-creative.o: main-creative.c
	$(CC) -c $(CFLAGS) $< -o $@

obj/%.o : src/%.c
	$(CC) -c $(CFLAGS) $< -o $@


clean: 
	rm -f obj/*.o


.PHONY: clean