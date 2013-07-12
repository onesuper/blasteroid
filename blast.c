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


void blasts_init(Blast b[], int size) {
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

void blasts_fire(Blast b[], int size, Spaceship* s, ALLEGRO_SAMPLE* fire) {
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


void blasts_move(Blast b[], int size, int width, int height) {
    int i;
    for (i = 0; i < size; i++) {
        if (b[i].live) {

            b[i].sx += b[i].speed * sin(b[i].heading);
            b[i].sy -= b[i].speed * cos(b[i].heading);
            b[i].bbox.center.x = b[i].sx;
            b[i].bbox.center.y = b[i].sy;
            
            if (b[i].sx > width || b[i].sx < 0 || b[i].sy > height || b[i].sy < 0)
                b[i].live = 0;  // out of screen
        }
    }
}


void blasts_draw(Blast b[], int size) {
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


void blasts_collide(Blast b[], int b_size, Asteroid a[], int a_size, ALLEGRO_SAMPLE* bang) {
    int i, j;
    for (i = 0; i < b_size; i++) {
        if (b[i].live)                   /*  if blast is alive */
        {                 
            for (j = 0; j < a_size; j++) 
            {
                if (a[j].live)           /* if asteroid is alive */ 
                {         
                    if (bbox_overlap(a[j].bbox, b[i].bbox))
                    {                        
                        a[j].live = 0;
                        b[i].live = 0;   /* both are dead */
                        al_play_sample(bang, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }
                }
            }
        }
    }
}
