/*
 * blast.h -
 *
 *
 * By onesuper(onesuperclark@gmail.com)
 *
 * You are welcome to use, share, and improve this source code.
 *
 */

#include "spaceship.h"
#include "asteroid.h"
#include "bbox.h"

#define MAX_BLAST_NUM 10


#ifndef BLAST_H_
#define BLAST_H_

typedef struct {
    float sx;
    float sy;
    float heading;
    float speed;
    int live;
    ALLEGRO_COLOR color;
    Bbox bbox;
} Blast;



void blasts_init(Blast b[], int size);
void blasts_draw(Blast b[], int size);
void blasts_fire(Blast b[], int size, Spaceship* s, ALLEGRO_SAMPLE* bang);
void blasts_move(Blast b[], int size, int width, int height);
void blasts_collide(Blast b[], int sizeB, Asteroid a[], int sizeA, ALLEGRO_SAMPLE* bang);


#endif
