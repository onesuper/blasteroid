/*
 *
 * Tutorial & manual for Allegro 5: http://wiki.allegro.cc/
 * https://www.allegro.cc/manual/5/index.html
 */

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "spaceship.h"
#include "blast.h"
#include "asteroid.h"


#define FPS 60
#define WIDTH 640
#define HEIGHT 480

/* 5 keys for controlling the spaceship */
int keys[5] = {0, 0, 0, 0, 0};
enum KEYS {UP, DOWN, LEFT, RIGHT, SPACE};


int main(int argc, char **argv) {

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    int redraw = 1;
    int doexit = 0;


    /* Gasme characters */
    Spaceship ship;
    Blast blasts[MAX_BLAST_NUM];
    Asteroid asteroids[MAX_ASTEROID_NUM];

    
    /* Init Allegro */
    if (!al_init()) {
        fprintf(stderr, "fail to initialize allegro!\n");
        return -1;
    }


    /* For drawing lines, triangles... */
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "fail to initialize primitives addon!\n");
        return -1;
    }


    /* Install keyboard */
    if (!al_install_keyboard()) {
        fprintf(stderr, "fail to install keyboard!\n");
        return -1;
    }


    /* Get a timer */
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "fail to create a timer!\n");
        return -1;
    }


    /* Get a display of WIDTH * HEIGHT */
    display = al_create_display(WIDTH, HEIGHT);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);  /* rollback */
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
  

    /* Initialize game characters here */
    ship_init(&ship, WIDTH/2, HEIGHT/2);
    blasts_init(blasts, MAX_BLAST_NUM);
    asteroids_init(asteroids, MAX_ASTEROID_NUM);

    /* kick off the timer */
    al_start_timer(timer);

    while (!doexit) 
    {
        ALLEGRO_EVENT event;

       
        /* Fetch an event from the event queue */
        al_wait_for_event(event_queue, &event);
        
        
        /* Check and make dicision */
        if (event.type == ALLEGRO_EVENT_TIMER) {   /* make change */

            redraw = 1;

            /* logical */
			if(keys[UP])
			    ship_acc(&ship);
			if(keys[DOWN])
				ship_slowndown(&ship);
			if(keys[LEFT])
				ship_turn_left(&ship);
			if(keys[RIGHT])
				ship_turn_right(&ship);

            // Update anyway
            ship_move(&ship, WIDTH, HEIGHT);
            blasts_move(blasts, MAX_BLAST_NUM, WIDTH, HEIGHT);
            asteroids_appear(asteroids, MAX_ASTEROID_NUM, WIDTH, HEIGHT);
            asteroids_move(asteroids, MAX_ASTEROID_NUM, WIDTH, HEIGHT);
                    
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {  /* close */

            doexit = 1;
     
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {   /* key press */
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    keys[UP] = 1;
                    ship.accelerating = 1;                 /* accelerating */
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
                case ALLEGRO_KEY_SPACE:
                    keys[SPACE] = 1;
                    blasts_fire(blasts, MAX_BLAST_NUM, &ship);   /* fire up */
                    break;
            }
        } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    keys[UP] = 0;
                    ship.accelerating = 0;        /* accelerating cancelled */
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
                case ALLEGRO_KEY_SPACE:
                    keys[SPACE] = 0;
                    break;
            }
        }


        if (redraw && al_is_event_queue_empty(event_queue)) {
            
            redraw = 0;                      

            /* drawing area */
            ship_draw(&ship);
            blasts_draw(blasts, MAX_BLAST_NUM);
            asteroids_draw(asteroids, MAX_ASTEROID_NUM);

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
