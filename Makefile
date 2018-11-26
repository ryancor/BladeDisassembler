CC = gcc
CFLAGS = -I.
DEPS = inc/disass.h inc/helper.h inc/gui.h
OBJ = src/main.o src/disass.o src/helper.o src/gui.o

%.o: %.c $(DEPS)
	$(CC) `pkg-config --cflags gtk+-2.0` -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o blade $(OBJ) `pkg-config --libs gtk+-2.0`

clean:
	@rm -f src/*.o
