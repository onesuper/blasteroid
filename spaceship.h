/*
 * spaceship.h -
 *
 *
 * By onesuper(onesuperclark@gmail.com)
 *
 * You are welcome to use, share, and improve this source code.
 *
 */


#include "bbox.h"


#ifndef SPACESHIP_H_
#define SPACESHIP_H_

#define MAX_SHIP_SPEED 3
#define MIN_SHIP_SPEED 0.3
#define SHIP_SPEEDUP 0.08
#define SHIP_ROTATE_ANGLE 0.05



typedef struct {
    float sx;
    float sy;
    float heading;
    float speed;
    int lives;          
    int accelerating;     /* Is accelerating ? */
    ALLEGRO_COLOR color;
    Bbox bbox;             /* bounding box */
} Spaceship;



void ship_init(Spaceship* s, int x, int y);
void ship_draw(Spaceship s);  
void ship_move(Spaceship* s, int width, int height);
void ship_acc(Spaceship* s, ALLEGRO_SAMPLE* thrust);
void ship_slowndown(Spaceship* s);
void ship_turn_right(Spaceship* s);
void ship_turn_left(Spaceship* s);


#endif
