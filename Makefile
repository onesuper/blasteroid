CXX=gcc
CFLAGS=-Wall -g
LDFLAGS=-L/usr/lib -lallegro -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_main 
INCLUDE=-I. -I/usr/include/allegro5 

OBJS=blasteroids.o spaceship.o blast.o asteroid.o bbox.o utils.o
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

bbox.o: bbox.c
	$(CXX) -c bbox.c $(CFLAGS)

utils.o: utils.c
	$(CXX) -c utils.c $(CFLAGS)