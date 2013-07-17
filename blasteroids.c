/*
 * blasteroid.c - the core of the game
 *
 * The code in here listens for keypresses, runs a timer, and also tell all 
 * of other spaceships, rocks, and blasts to draw themselves on the screen.
 *
 * It is also in charge of the initializations of any resources that will
 * be used in the game, such as audio, font, bitmap, etc.
 *
 * By onesuper(onesuperclark@gmail.com)
 *
 * You are welcome to use, share, and improve this source code.
 *
 */

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "spaceship.h"
#include "asteroid.h"
#include "blast.h"
#include "bbox.h"
#include "misc.h"


#define FPS 60

/* 5 keys for controlling the spaceship */
int keys[5] = {0, 0, 0, 0, 0};
enum KEYS {UP, DOWN, LEFT, RIGHT, SPACE};


int main(int argc, char **argv) {


    /* Game resources/variables */
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_SAMPLE *sound_fire = NULL;
    ALLEGRO_SAMPLE *sound_bang = NULL;
    ALLEGRO_SAMPLE *sound_thrust = NULL;
    int redraw = 1;
    int doexit = 0;
    int score = 0;
    int asteroid_num = 4;


    /*******************************************************************
     *            Init & Install Audio/keyboard/Addons
     ******************************************************************/ 
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives addon!\n");
        return -1;
    }
    
    if(!al_init_acodec_addon()) {
        fprintf(stderr, "failed to initialize audio codecs!\n");
        return -1;
    }

    /* Install keyboard */
    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to install keyboard!\n");
        return -1;
    }


    /***********************************************************************
     *   Game audio
     **********************************************************************/
    if (!al_install_audio()){
        fprintf(stderr, "failed to install audio!\n");
        return -1;
    }
    
    if (!al_reserve_samples(3)){
        fprintf(stderr, "failed to reserve samples!\n");
        return -1;
    }
    
    sound_fire = al_load_sample("sound/fire.wav"); 
    if (!sound_fire){
        fprintf(stderr, "fire.wav not loaded!\n" ); 
        return -1;
    }

    sound_bang = al_load_sample("sound/bang.wav"); 
    if (!sound_bang){
        fprintf(stderr, "bang.wav not loaded!\n" ); 
        return -1;
    }

    sound_thrust = al_load_sample("sound/thrust.wav"); 
    if (!sound_thrust){
        fprintf(stderr, "thrust.wav not loaded!\n" ); 
        return -1;
    }


    /* Get a timer */
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "failed to create a timer!\n");
        return -1;
    }


    /* Get a display of WIDTH * HEIGHT */
    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
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
  

    /* Init our game characters here */
    Spaceship ship;
    ship_init(&ship);

    Blast blasts_array[MAX_BLAST_NUM];  
    blast_init(blasts_array, MAX_BLAST_NUM);

    Asteroid *asteroids_list = asteroid_init();
    asteroid_append(asteroids_list, 4);



    /* kick off the timer */
    al_start_timer(timer);


    while (!doexit) 
    {
        ALLEGRO_EVENT event;

       
        /* Fetch an event from the event queue */
        al_wait_for_event(event_queue, &event);
        
        
        /* Tick! Check everything, make dicisions, and redraw  */
        if (event.type == ALLEGRO_EVENT_TIMER) {  

            redraw = 1;

            /* Continuous Effect */
			if (keys[UP]) {
			    ship_acc(&ship, sound_thrust);
                ship.accelerating = 1;                   /* accelerating */
            } else {
                ship.accelerating = 0;
            } 
			if (keys[DOWN])
				ship_slowndown(&ship);
			if (keys[LEFT])
				ship_turn_left(&ship);
			if (keys[RIGHT])
				ship_turn_right(&ship);
                

            /* Update anyway */
            ship_move(&ship);
            blast_move(blasts_array, MAX_BLAST_NUM);
            asteroid_move(asteroids_list);            
            asteroid_collide(asteroids_list, &ship, sound_bang, &asteroid_num);      
            blast_collide(blasts_array, MAX_BLAST_NUM, asteroids_list, sound_bang, &score, &asteroid_num);                    

            /* Main Game Logic */

            if (asteroid_num == 0) {
                
                asteroid_append(asteroids_list, 4);
                asteroid_num += 4;
            }

       
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
                case ALLEGRO_KEY_SPACE:
                    keys[SPACE] = 1;
                    blast_fire(blasts_array, MAX_BLAST_NUM, &ship, sound_fire);    /* fire! */                    
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
                case ALLEGRO_KEY_SPACE:
                    keys[SPACE] = 0;
                    break;
            }
        }


        if (redraw && al_is_event_queue_empty(event_queue)) {
            
            redraw = 0;                      

            /* drawing area */
            ship_draw(ship);
            blast_draw(blasts_array, MAX_BLAST_NUM);
            asteroid_draw(asteroids_list);
#if 0
            /* Bounding Box test, delete later */
            bbox_draw(ship.bbox);
            int i;
            for (i = 0; i < MAX_ASTEROID_NUM; i++) {
                if (asteroids[i].live)
                    bbox_draw(asteroids[i].bbox);
            }
            for (i = 0; i < MAX_BLAST_NUM; i++) {
                if (blasts_array[i].live)
                    bbox_draw(blasts[i].bbox);
            }
#endif
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }


    /* Destroy everything here */
    asteroid_destroy(asteroids_list);
    al_destroy_sample(sound_fire);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    
    return 0;
}
