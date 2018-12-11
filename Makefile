CC = gcc
CXX = g++
CFLAGS = -I. -O0 -O2
CXXFLAGS = -std=c++14 -O0 -O2 -Wno-gnu-string-literal-operator-template
DEPS = inc/disass.h inc/helper.h inc/gui.h inc/obfuscate.h
OBJ = src/main src/disass src/helper src/gui src/obfuscate
OBJS = $(addsuffix .o,$(OBJ))

all:
	make compile

compile:
	make $(OBJS)
	make blade

blade: $(OBJS)
	$(CXX) -o blade $(OBJS) `pkg-config --libs gtk+-2.0`

# compile rest of the C files
%.o: %.c $(DEPS)
	$(CC) `pkg-config --cflags gtk+-2.0` -c -o $@ $< $(CFLAGS) 

# compile the cpp file on its own
obfuscate.o: obfuscate.cpp
		$(CXX) $(CXXFLAGS) -c -o src/obfuscate.o src/obfuscate.cpp

clean:
	@rm -f src/*.o
