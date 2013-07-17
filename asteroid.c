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
#include "misc.h"

int asteroid_points[3] = {20, 50, 100};
float asteroid_scale[3] = {1.5, 1.25, 1.0}; 
enum ASTEROID_TYPE {LARGE, MEDIUM, SMALL};



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
Asteroid *asteroid_create() {

    Asteroid *a = malloc(sizeof(Asteroid));

    a->sx = 30;
    a->sy = 30 + rand() % (SCREEN_HEIGHT -60);
    a->speed =0.5;
    a->rot_velocity = 0.01;
    a->heading = (float)rand()/((float)RAND_MAX/10.0);
    a->twist = (float)rand()/((float)RAND_MAX/10.0);
    a->type = LARGE;                    /* large rocks at the beginning */ 
    a->scale = asteroid_scale[LARGE];
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
 * Append n asteroids to the head of list
 */
void asteroid_append(Asteroid *head, int n) {
   
    int i;
    for (i = 0; i < n; i++) {        
        Asteroid *new = asteroid_create();
        new->next = head->next;
        head->next = new;
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
            s->lives--;

            /* bang */
            al_play_sample(bang, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        } 


    }
}



