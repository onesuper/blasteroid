/*
 * test_collision.c - verify the correctness of the collision test module
 *
 * It will create two bounding boxes on the screen. One of them can be 
 * moved around(left/right/up/down). Both of them can be rotated 
 * clockwise(W/X) or anticlockwise(Q/Z). 
 *
 * By onesuper(onesuperclark@gmail.com)
 *
 * You are welcome to use, share, and improve this source code.
 *
 */


#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "bbox.h"


#define FPS 60
#define WIDTH 640
#define HEIGHT 480



/* 5 keys for controlling the spaceship */
int keys[8] = {0, 0, 0, 0, 0, 0, 0, 0};
enum KEYS {UP, DOWN, LEFT, RIGHT, Q, W, Z, X};



int main(int argc, char **argv) {


    /* Game engine resources */
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;


    int redraw = 1;
    int doexit = 0;
    int overlap = 0;

    Bbox b1, b2;
    b1.center.x = WIDTH / 2;
    b1.center.y = HEIGHT / 2;
    b2.center.x = b1.center.x + 100;
    b2.center.y = b1.center.y + 100;

    b1.heading = 0.0;
    b2.heading = 0.0;

    b1.left = 2;
    b1.right = 2;
    b1.top = 3;
    b1.bottom = 2;

    b2.left = 20;
    b2.right = 20;
    b2.top = 30;
    b2.bottom = 30;

    /* Init Allegro */
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }
    
    /* NOTE: it's necessary to call al_map_rgb after al_init */
    b1.color = al_map_rgb(230, 230, 230);
    b2.color = al_map_rgb(230, 230, 230);


    /*******************************************************************
     *                         Addons
     ******************************************************************/    
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives addon!\n");
        return -1;
    }
   

    /* Install keyboard */
    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to install keyboard!\n");
        return -1;
    }



    /* Get a timer */
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "failed to create a timer!\n");
        return -1;
    }


    /* Get a display of WIDTH * HEIGHT */
    display = al_create_display(WIDTH, HEIGHT);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);        /* rollback */
        return -1;
    }


    /* Get an event queue */
    event_queue = al_create_event_queue();    
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);   /* rollback 1 */
        al_destroy_timer(timer);       /* rollback 2 */
        return -1;
    }


    /* Registering everything in event queue */
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));


    /* Set background black */
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
  

    /* kick off the timer */
    al_start_timer(timer);


    while (!doexit) 
    {
        ALLEGRO_EVENT event;

       
        /* Fetch an event from the event queue */
        al_wait_for_event(event_queue, &event);
        
        
        /* Check and make dicision */
        if (event.type == ALLEGRO_EVENT_TIMER) {   

            redraw = 1;

            /* move the b1 around */
			if(keys[UP])
			    b1.center.y -= 5;
			if(keys[DOWN])
				b1.center.y += 5;
			if(keys[LEFT])
				b1.center.x -= 5;
			if(keys[RIGHT])
				b1.center.x += 5;         
            if(keys[Q])
				b1.heading -= 0.05;  
            if(keys[W])
				b1.heading += 0.05;  
            if(keys[Z])
				b2.heading -= 0.05;  
            if(keys[X])
				b2.heading += 0.05;  

            /* determine overlap */
            overlap = bbox_overlap(b1, b2);


        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {  /* close */

            doexit = 1;
     
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {   /* key press */
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    keys[UP] = 1;
                    break;
                case ALLEGRO_KEY_DOWN:
                    keys[DOWN] = 1;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[RIGHT] = 1;
                    break;
                case ALLEGRO_KEY_LEFT:
                    keys[LEFT] = 1;
                    break;
                case ALLEGRO_KEY_Z:
                    keys[Z] = 1;
                    break;
                case ALLEGRO_KEY_X:
                    keys[X] = 1;
                    break;
                case ALLEGRO_KEY_Q:
                    keys[Q] = 1;
                    break;
                case ALLEGRO_KEY_W:
                    keys[W] = 1;
                    break;
            }
        } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    keys[UP] = 0;                   
                    break;
                case ALLEGRO_KEY_DOWN:
                    keys[DOWN] = 0;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[RIGHT] = 0;
                    break;
                case ALLEGRO_KEY_LEFT:
                    keys[LEFT] = 0;
                    break;
                case ALLEGRO_KEY_Z:
                    keys[Z] = 0;
                    break;
                case ALLEGRO_KEY_X:
                    keys[X] = 0;
                    break;
                case ALLEGRO_KEY_Q:
                    keys[Q] = 0;
                    break;
                case ALLEGRO_KEY_W:
                    keys[W] = 0;
                    break;
            }
        }


        if (redraw && al_is_event_queue_empty(event_queue)) {
            
            redraw = 0;                      

            /* drawing area */
            bbox_draw(b1);
            bbox_draw(b2);

            if (overlap) {
                ALLEGRO_TRANSFORM transform;
                al_identity_transform(&transform);
                al_translate_transform(&transform, 20, 20);
                al_use_transform(&transform);
                al_draw_filled_circle(0, 0, 20, al_map_rgb(255, 0, 0));
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }


    /* Destroy everything here */

    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    
    return 0;
}






