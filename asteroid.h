/*
 * asteroid.h - 
 *
 *
 * By onesuper(onesuperclark@gmail.com)
 *
 * You are welcome to use, share, and improve this source code.
 *
 */

#include "spaceship.h"

#ifndef ASTEROID_H_
#define ASTEROID_H_

#define MAX_ASTEROID_NUM 4



typedef struct asteroid_t{    
    float sx;
    float sy;
    int type;
    float heading;
    float twist;
    float speed;
    float rot_velocity;
    float scale;
    ALLEGRO_COLOR color;    
    Bbox bbox;
    struct asteroid_t *next;
} Asteroid;

enum ASTEROID_TYPE {LARGE, MEDIUM, SMALL};

Asteroid *asteroid_init(void);
Asteroid *asteroid_create(int type);
void asteroid_fill_list(Asteroid *a, int n);
void asteroid_destroy(Asteroid *a);
void asteroid_move(Asteroid *a);
void asteroid_draw(Asteroid *a);
void asteroid_collide(Asteroid *a, Spaceship *s, ALLEGRO_SAMPLE *bang, int *asteroid_num);
int asteroid_split(Asteroid *a);

#endif
