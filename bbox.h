/*
 * bbox.h - bounding box header file
 *
 *
 * By onesuper(onesuperclark@gmail.com)
 *
 * You are welcome to use, share, and improve this source code.
 *
 */


#include "utils.h"

#ifndef BBOX_H_
#define BBOX_H_


typedef struct {
    Point center; 
    float heading;
    ALLEGRO_COLOR color;
    float top;          /* distance between border and center */
    float right;
    float bottom;
    float left;
} Bbox;



void bbox_draw(Bbox b);
int bbox_overlap(Bbox b1, Bbox b2);



#endif
