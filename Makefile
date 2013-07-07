CXX=gcc
CFLAGS=-Wall
LDFLAGS=-L/usr/lib -lallegro -lallegro_primitives
INCLUDE=-I. -I/usr/include/allegro5 

OBJS=blasteroids.o spaceship.o blast.o asteroid.o
OUT=blasteroid

all: blasteroid

clean:
	rm -rf *.o blasteroid

blasteroid: $(OBJS)
	$(CXX) $(OBJS) -o $(OUT) $(INCLUDE) $(CFLAGS) $(LDFLAGS)

blasteroids.o: blasteroids.c
	$(CXX) -c blasteroids.c $(CFLAGS)

spaceship.o: spaceship.c
	$(CXX) -c spaceship.c $(CFLAGS)

blast.o: blast.c
	$(CXX) -c blast.c $(CFLAGS)

asteroid.o: asteroid.c
	$(CXX) -c asteroid.c $(CFLAGS)