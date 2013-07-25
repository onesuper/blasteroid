/*
 * spaceship.c - the hero of our game
 *
 *
 * By onesuper(onesuperclark@gmail.com)
 *
 * You are welcome to use, share, and improve this source code.
 *
 */


#include <math.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include "spaceship.h"
#include "misc.h"

void ship_init(Spaceship *s) {
    s->sx = SCREEN_WIDTH/2;
    s->sy = SCREEN_HEIGHT/2;
    s->heading = 0;
    s->speed = 0;
    s->lives = 3;
    s->accelerating = 0;
    s->color = al_map_rgb(0, 255, 255);  
    s->bbox.center.x = SCREEN_WIDTH/2;
    s->bbox.center.y = SCREEN_HEIGHT/2;
    s->bbox.left = 10;
    s->bbox.right = 10;
    s->bbox.top= 12;
    s->bbox.bottom =10;
    s->bbox.heading = 0;
    s->bbox.color = al_map_rgb(255, 255, 255);  
}


void ship_draw(Spaceship s) {

    if (s.lives <= 1)
        return;

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



void ship_damage(Spaceship *s) {

    s->lives--;

    if (s->lives > 0) {
        s->sx = SCREEN_WIDTH / 2;
        s->sy = SCREEN_HEIGHT / 2;
        s->heading = 0;
        s->speed = 0;
    }
}

void ship_move(Spaceship *s) {

    if (s->lives < 1)
        return;

    s->sy -= s->speed * cos(s->heading);
    s->sx += s->speed * sin(s->heading);
    
    /* loop boundary */
    if (s->sy < 0)
        s->sy += SCREEN_WIDTH;
    if (s->sy > SCREEN_HEIGHT)
        s->sy -= SCREEN_HEIGHT;
    if (s->sx < 0)
        s->sx += SCREEN_WIDTH;
    if (s->sx > SCREEN_WIDTH) 
        s->sx -= SCREEN_WIDTH;

    /* move and rotate the bounding box */
    s->bbox.center.x = s->sx;
    s->bbox.center.y = s->sy;
    s->bbox.heading = s->heading;
}

void ship_acc(Spaceship *s, ALLEGRO_SAMPLE *thrust) {
    s->speed += SHIP_SPEEDUP;
    if (s->speed > MAX_SHIP_SPEED)
        s->speed = MAX_SHIP_SPEED;
    al_play_sample(thrust, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}


void ship_slowndown(Spaceship *s) {
    s->speed -= SHIP_SPEEDUP;
    if (s->speed < MIN_SHIP_SPEED)
        s->speed = MIN_SHIP_SPEED;
}

void ship_turn_right(Spaceship *s) {
    s->heading += SHIP_ROTATE_ANGLE;
}

void ship_turn_left(Spaceship *s) {
    s->heading -= SHIP_ROTATE_ANGLE;
}

