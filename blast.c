/*
 * blast.c - the bullet
 *
 * When the spaceship fire the bullet, the code will need to draw and move
 * a blast across the screen. If a blast is out of the screen, then it's 
 * gone.
 *
 * By onesuper(onesuperclark@gmail.com)
 *
 * You are welcome to use, share, and improve this source code.
 *
 */


#include <math.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include "blast.h"
#include "misc.h"

void blast_init(Blast b[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        b[i].speed = 5;
        b[i].live = 0;        /* all blasts are dead until they are fired */
        b[i].bbox.top = 2;
        b[i].bbox.left = 2;
        b[i].bbox.right = 2;
        b[i].bbox.bottom = 2;
        b[i].bbox.color = al_map_rgb(255,0,0);
    }
}

void blast_fire(Blast b[], int size, Spaceship *s, ALLEGRO_SAMPLE *fire) {
    int i;
    for (i = 0; i < size; i++) {
        if (!b[i].live) {
            b[i].sx = s->sx + 5 * sin(s->heading);
            b[i].sy = s->sy - 5 * cos(s->heading);
            b[i].heading = s->heading;   /* the blast fired heads to the same direction as the spaceship */
            b[i].live = 1;
            b[i].bbox.center.x = b[i].sx;
            b[i].bbox.center.y = b[i].sy;
            al_play_sample(fire, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;
        }
    }
}


void blast_move(Blast b[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (b[i].live) {

            b[i].sx += b[i].speed * sin(b[i].heading);
            b[i].sy -= b[i].speed * cos(b[i].heading);
            b[i].bbox.center.x = b[i].sx;
            b[i].bbox.center.y = b[i].sy;
            
            if (b[i].sx > SCREEN_WIDTH || b[i].sx < 0 || b[i].sy > SCREEN_HEIGHT || b[i].sy < 0)
                b[i].live = 0;  // out of screen
        }
    }
}


void blast_draw(Blast b[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (b[i].live) {
            ALLEGRO_TRANSFORM transform;
            al_identity_transform(&transform);
            al_rotate_transform(&transform, b[i].heading);
            al_translate_transform(&transform, b[i].sx, b[i].sy);
            al_use_transform(&transform);

            al_draw_filled_circle(0, 0, 2, al_map_rgb(255, 255, 255));
        }
    }
}


void blast_collide(Blast b[], int b_size, Asteroid *a, ALLEGRO_SAMPLE *bang, int *score, int *asteroid_num) {

    Asteroid *prev, *p;

    /* 
     * go through all the bullets
     */
    int i;
    for (i = 0; i < b_size; i++) {
        if (b[i].live)                   /*  only check the alive blast */
        {
            /*
             * go through the asteroid list
             */
            for (prev = a, p = a->next;
                 p != NULL;
                 prev = p, p = p->next) 
            { 
                if (bbox_overlap(p->bbox, b[i].bbox))
                {   

                    /* kill or split the Asteroid */
                    if (prev) {
                        prev->next = p->next;
                        free(p);
                        *asteroid_num -= 1;
                    }
                    
                    /* kill the blast */
                    b[i].live = 0;
                    
                    /* bang */
                    al_play_sample(bang, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

                    /* increse the score */
                }
                
            }
        }
    }

}
