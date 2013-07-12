/*
 * asteroid.c - the big enemies in the game
 * 
 * By onesuper(onesuperclark@gmail.com)
 *
 * You are welcome to use, share, and improve this source code.
 *
 */

#include <math.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include "asteroid.h"


void asteroids_init(Asteroid a[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        a[i].live = 0;
        a[i].speed = 0.5;
        a[i].rot_velocity = 0.01;
        a[i].twist = 0;        
        a[i].color = al_map_rgb(220,220,220);
        a[i].bbox.top = 20; 
        a[i].bbox.bottom = 20;
        a[i].bbox.left = 25;
        a[i].bbox.right = 20;
        a[i].bbox.heading = 0;
        a[i].bbox.color = al_map_rgb(255,255,255);
    }
}

void asteroids_appear(Asteroid a[], int size, int width, int height) {
    int i;

    for (i = 0; i < size; i++) {
        if(!a[i].live) {
            a[i].heading = (float)rand()/((float)RAND_MAX/10.0);
            a[i].twist = (float)rand()/((float)RAND_MAX/10.0);
            a[i].scale = (float)rand()/((float)RAND_MAX/0.5) + 1;
            a[i].live = 1;
            a[i].sx = 30;
            a[i].sy = 30 + rand() % (height -60);
            a[i].bbox.center.x = a[i].sx;
            a[i].bbox.center.y = a[i].sy;  
            a[i].bbox.top = 20 * a[i].scale;   
            a[i].bbox.left = 25 * a[i].scale;
            a[i].bbox.right = 20 * a[i].scale;
            a[i].bbox.bottom = 20 * a[i].scale;
            a[i].bbox.heading = a[i].heading;
            break;                
        }        
    }

}

void asteroids_move(Asteroid a[], int size, int width, int height) {
    int i;   
    for (i = 0; i < size; i++) {
        if (a[i].live) {
            a[i].sx += a[i].speed * sin(a[i].heading);
            a[i].sy -= a[i].speed * cos(a[i].heading);
            a[i].twist += a[i].rot_velocity;        /* rotate it */

            /* loop boundary */
            if (a[i].sx < 0) 
                a[i].sx += width;
            if (a[i].sx > width) 
                a[i].sx -= width;
            if (a[i].sy < 0)
                a[i].sy += height;
            if (a[i].sy > height)
                a[i].sy -= height;

            a[i].bbox.center.x = a[i].sx;
            a[i].bbox.center.y = a[i].sy;
            a[i].bbox.heading = a[i].twist;
        }
    }
}

void asteroids_draw(Asteroid a[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (a[i].live) {
            
            ALLEGRO_TRANSFORM transform;
            al_identity_transform(&transform);
            al_scale_transform(&transform, a[i].scale, a[i].scale);
            al_rotate_transform(&transform, a[i].twist);  /* twist the angle */
            al_translate_transform(&transform, a[i].sx, a[i].sy);
            al_use_transform(&transform);

            
            al_draw_line(-20,  20, -25,   5, a[i].color, 2.0f);
            al_draw_line(-25,   5, -25, -10, a[i].color, 2.0f);
            al_draw_line(-25, -10,  -5, -10, a[i].color, 2.0f);
            al_draw_line( -5, -10, -10, -20, a[i].color, 2.0f);
            al_draw_line(-10, -20,   5, -20, a[i].color, 2.0f);
            al_draw_line(  5, -20,  20, -10, a[i].color, 2.0f);
            al_draw_line( 20, -10,  20,  -5, a[i].color, 2.0f);
            al_draw_line( 20,  -5,   0,   0, a[i].color, 2.0f);
            al_draw_line(  0,   0,  20,  10, a[i].color, 2.0f);
            al_draw_line( 20,  10,  10,  20, a[i].color, 2.0f);
            al_draw_line( 10,  20,   0,  15, a[i].color, 2.0f);
            al_draw_line(  0,  15, -20,  20, a[i].color, 2.0f);
        }
        
    }

}


void asteroids_collide(Asteroid a[], int size, Spaceship* s, ALLEGRO_SAMPLE* bang) {
    int i;
    for (i = 0; i < size; i++) {
        if (bbox_overlap(a[i].bbox, s->bbox)) {
            a[i].live = 0;
            s->lives--;
            al_play_sample(bang, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        }                      
 
    }
}

