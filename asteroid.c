/*
 * asteroid.c - the big enemies in the game
 * 
 * By onesuper(onesuperclark@gmail.com)
 *
 * You are welcome to use, share, and improve this source code.
 *
 */


#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include "asteroid.h"
#include "misc.h"
#include <math.h>


float asteroid_scale[3] = {1.25, 0.9, 0.4}; 


/*
 * Get a empty list, return a pointer to it
 */
Asteroid *asteroid_init() {
    Asteroid *head = malloc(sizeof(Asteroid));
    head->next = NULL;
    return head;
}


/*
 * Give birth to a asteroid
 */
Asteroid *asteroid_create(int type) {

    Asteroid *a = malloc(sizeof(Asteroid));

    a->sx = 30;
    a->sy = 30 + rand() % (SCREEN_HEIGHT -60);
    a->speed =0.5 + random_float(1.5);
    a->rot_velocity = 0.01;
    a->heading = random_float(10.0);
    a->twist = random_float(10.0);
    a->type = type;                    /* large rocks at the beginning */ 
    a->scale = asteroid_scale[type];
    a->color = al_map_rgb(220,220,220);
    a->bbox.top = 20 * a->scale; 
    a->bbox.bottom = 20 * a->scale;
    a->bbox.left = 25 * a->scale;
    a->bbox.right = 20 * a->scale;    
    a->bbox.color = al_map_rgb(255,255,255);
    a->bbox.center.x = a->sx;
    a->bbox.center.y = a->sy;  
    a->bbox.heading = a->heading;
    a->next = NULL;

    return a;
}


/*
 * fill a list with n LARGE Asteroids
 */
void asteroid_fill_list(Asteroid *a, int n) {
    Asteroid *new;
    int i;
    for (i = 0; i < n; i++) {        
        new = asteroid_create(LARGE);
        new->next = a->next;
        a->next = new;
    }

}


void asteroid_destroy(Asteroid *head) {

    Asteroid *p, *prev;
    for (prev = NULL, p=head;
         p != NULL;
         prev = p, p = p->next)
    {
        free(prev);
    }
}


void asteroid_move(Asteroid *a) {

    Asteroid *p;

    for (p = a->next; p != NULL ; p = p->next) {
        
        p->sx += p->speed * sin(p->heading);
        p->sy -= p->speed * cos(p->heading);
        p->twist += p->rot_velocity;        /* rotate it */

        /* loop boundary */
        if (p->sx < 0) 
            p->sx += SCREEN_WIDTH;
        if (p->sx > SCREEN_WIDTH) 
            p->sx -= SCREEN_WIDTH;
        if (p->sy < 0)
            p->sy += SCREEN_HEIGHT;
        if (p->sy > SCREEN_HEIGHT)
            p->sy -= SCREEN_HEIGHT;

        p->bbox.center.x = p->sx;
        p->bbox.center.y = p->sy;
        p->bbox.heading = p->twist;      
    }
}

void asteroid_draw(Asteroid *a) {

    Asteroid *p;

    for (p = a->next; p != NULL ; p = p->next) {       
                 
        ALLEGRO_TRANSFORM transform;
        al_identity_transform(&transform);
        al_scale_transform(&transform, p->scale, p->scale);
        al_rotate_transform(&transform, p->twist);  /* twist the angle */
        al_translate_transform(&transform, p->sx, p->sy);
        al_use_transform(&transform);
            
        al_draw_line(-20,  20, -25,   5, p->color, 2.0f);
        al_draw_line(-25,   5, -25, -10, p->color, 2.0f);
        al_draw_line(-25, -10,  -5, -10, p->color, 2.0f);
        al_draw_line( -5, -10, -10, -20, p->color, 2.0f);
        al_draw_line(-10, -20,   5, -20, p->color, 2.0f);
        al_draw_line(  5, -20,  20, -10, p->color, 2.0f);
        al_draw_line( 20, -10,  20,  -5, p->color, 2.0f);
        al_draw_line( 20,  -5,   0,   0, p->color, 2.0f);
        al_draw_line(  0,   0,  20,  10, p->color, 2.0f);
        al_draw_line( 20,  10,  10,  20, p->color, 2.0f);
        al_draw_line( 10,  20,   0,  15, p->color, 2.0f);
        al_draw_line(  0,  15, -20,  20, p->color, 2.0f);
              
    }
}


void asteroid_collide(Asteroid *a, Spaceship *s, ALLEGRO_SAMPLE *bang, int *asteroid_num) {

    if (s->lives < 1) 
        return;

    Asteroid *prev, *p;

    for (prev = a, p = a->next;
         p != NULL;
         prev = p, p = p->next) {

        if (bbox_overlap(p->bbox, s->bbox)) {
            
            /* kill the asteroid */
            if (prev) {
                prev->next = p->next;
                free(p);
                *asteroid_num -= 1;
            }

            /* kill the spaceship */
            ship_damage(s);

            /* bang */
            al_play_sample(bang, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        } 


    }
}


/*
 * it returns whether the asteroid split
 */
int asteroid_split(Asteroid *a) {
    
    if (a->type == LARGE) {
        Asteroid *new1 = asteroid_create(MEDIUM);
        Asteroid *new2 = asteroid_create(MEDIUM);
        new1->sx = new2->sx = a->sx;
        new1->sy = new2->sy = a->sy;
        new1->next = new2;
        new2->next = a->next;
        a->next = new1;
        return 1;
        
    } else if (a->type == MEDIUM) {
        Asteroid *new1 = asteroid_create(SMALL);
        Asteroid *new2 = asteroid_create(SMALL);
        new1->sx = new2->sx = a->sx;
        new1->sy = new2->sy = a->sy;
        new1->next = new2;
        new2->next = a->next;
        a->next = new1;
        return 1;
    } else {
        return 0;  /* too SMALL to split */
    }   
}

