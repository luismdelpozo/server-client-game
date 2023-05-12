CC = gcc
CFLAGS = -Wall
OBJECTS = memory.o game.o map.o player.o ship.o
all = prog

prog: $(OBJECTS)
			$(CC) $(OBJECTS) -o prog

%.o: %.c
		$(CC) $(CFLAGS) $<

clean:
		rm -rf *.o