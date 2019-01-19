all: test_driver.o linkedlist.o game_setup.o tiles.o board.o player.o dict.o
	gcc -o runthis test_driver.o linkedlist.o game_setup.o tiles.o board.o player.o dict.o

test_driver.o: test_driver.c networking.h
	gcc -c test_driver.c

linkedlist.o: linkedlist.c linkedlist.h
	gcc -c linkedlist.c

game_setup.o: game_setup.c game_setup.h
	gcc -c game_setup.c

tiles.o: tiles.c tiles.h
	gcc -c tiles.c

board.o: board.c board.h
	gcc -c board.c

player.o: player.c player.h
	gcc -c player.c

dict.o: dict.c dict.h
	gcc -c dict.c
	
run:
	./runthis

clean:
	rm runthis
	rm *.o
