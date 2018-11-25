CC = gcc
CFLAGS = -I.
DEPS = inc/disass.h inc/helper.h
OBJ = src/main.o src/disass.o src/helper.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o blade $(OBJ)

clean:
	@rm -f src/*.o
