CC=gcc
CFLAGS=-I.
DEPS = inc/disass.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: src/main.o src/disass.o
	$(CC) -o blade src/main.o src/disass.o

clean:
	@rm -f src/*.o
