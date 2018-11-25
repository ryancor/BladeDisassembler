CC=gcc
CFLAGS=-I.
DEPS = disass.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o src/disass.o
	$(CC) -o blade main.o src/disass.o

clean:
	@rm -f *.o src/*.o
