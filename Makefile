CC=gcc
CFLAGS=-std=c99 -I. -I.. -ggdb `sdl-config --cflags`
LDFLAGS=-ldl -lGL -lGLU `sdl-config --libs`
GENS=gens/circle.so gens/shell.so gens/logspiral.so gens/sqrtspiral.so gens/rotor.so \
	gens/star1.so gens/star2.so gens/star3.so gens/dragon.so

all: shinymath $(GENS)

shinymath: shinymath.c shinymath.h
	$(CC) shinymath.c $(CFLAGS) -o shinymath $(LDFLAGS)

%.so: %.c gens/utils.h
	$(CC) -shared gens/utils.c $< -o $@

clean:
	rm -f shinymath $(GENS)
