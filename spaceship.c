
#include <math.h>
#include <allegro5/allegro_primitives.h>
#include "spaceship.h"


void ship_init(Spaceship* s, int x, int y) {
    s->sx = x;
    s->sy = y;
    s->heading = 0;
    s->speed = 0;
    s->lives = 3;
    s->accelerating = 0;
    s->color = al_map_rgb(0, 255, 255);  
    s->bbox.center.x = x;
    s->bbox.center.y = y;
    s->bbox.left = 10;
    s->bbox.right = 10;
    s->bbox.top= 12;
    s->bbox.bottom =10;
    s->bbox.heading = 0;
}

void ship_draw(Spaceship s) {
    
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_rotate_transform(&transform, s.heading);
    al_translate_transform(&transform, s.sx, s.sy);
    al_use_transform(&transform);
    
    al_draw_line(-8,   9,  0, -11, s.color, 3.0f);
    al_draw_line( 0, -11,  8,   9, s.color, 3.0f);
    al_draw_line(-6,   4, -1,   4, s.color, 3.0f);
    al_draw_line( 6,   4,  1,   4, s.color, 3.0f);

    if (s.accelerating) {
        al_draw_line(-3, 5, -3, 15, al_map_rgb(255, 0, 0), 2.0f);
        al_draw_line( 3, 5,  3, 15, al_map_rgb(255, 0, 0), 2.0f);
    }
    
}


void ship_move(Spaceship* s, int width, int height) {

    s->sy -= s->speed * cos(s->heading);
    s->sx += s->speed * sin(s->heading);
    
    /* loop boundary */
    if (s->sy < 0)
        s->sy += height;
    if (s->sy > height)
        s->sy -= height;
    if (s->sx < 0)
        s->sx += width;
    if (s->sx > width) 
        s->sx -= width;

    /* move and rotate the bounding box */
    s->bbox.center.x = s->sx;
    s->bbox.center.y = s->sy;
    s->bbox.heading = s->heading;
}

void ship_acc(Spaceship* s) {
    s->speed += SHIP_SPEEDUP;
    if (s->speed > MAX_SHIP_SPEED)
        s->speed = MAX_SHIP_SPEED;
}


void ship_slowndown(Spaceship* s) {
    s->speed -= SHIP_SPEEDUP;
    if (s->speed < MIN_SHIP_SPEED)
        s->speed = MIN_SHIP_SPEED;
}

void ship_turn_right(Spaceship* s) {
    s->heading += SHIP_ROTATE_ANGLE;
}

void ship_turn_left(Spaceship* s) {
    s->heading -= SHIP_ROTATE_ANGLE;
}

