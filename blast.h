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



void blast_init(Blast b[], int size);
void blast_draw(Blast b[], int size);
void blast_fire(Blast b[], int size, Spaceship *s, ALLEGRO_SAMPLE *bang);
void blast_move(Blast b[], int size);
void blast_collide(Blast b[], int sizeB, Asteroid *a,
                    ALLEGRO_SAMPLE *bang, int *score, int *asteroid_num);


#endif
