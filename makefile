all: driver.o linkedlist.o game_setup.o tiles.o board.o
	gcc -o runthis driver.o linkedlist.o game_setup.o tiles.o board.o

driver.o: driver.c driver.h
	gcc -c driver.c

linkedlist.o: linkedlist.c linkedlist.h
	gcc -c linkedlist.c

game_setup.o: game_setup.c game_setup.h
	gcc -c game_setup.c

tiles.o: tiles.c tiles.h
	gcc -c tiles.c

board.o: board.c board.h
	gcc -c board.c

run:
	./runthis

clean:
	rm runthis
	rm *.o
