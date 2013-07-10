
#include "utils.h"

#ifndef BBOX_H_
#define BBOX_H_


typedef struct {
    Point center; 
    float heading;
    float top;          /* distance between border and center */
    float right;
    float bottom;
    float left;
} Bbox;



void bbox_draw(Bbox b);
int bbox_collide(Bbox b1, Bbox b2);






#endif
