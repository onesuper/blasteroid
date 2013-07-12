/*
 * utils.c - some geometric stuff 
 *
 * By onesuper(onesuperclark@gmail.com)
 *
 * You are welcome to use, share, and improve this source code.
 *
 */

#include <math.h>
#include "utils.h"

void point_rotate(Point* p, Point center, float theta) {
    
   

    float x = p->x;
    float y = p->y;

    p->x = center.x + (x - center.x) * cos(-theta) + (y - center.y) * sin(-theta);
    p->y = center.y - (x - center.x) * sin(-theta) + (y - center.y) * cos(-theta);

}


Point project(Point A, Point B) {
    Point p;
    float scalar = A.x * B.x + A.y * B.y;
    scalar /= B.x * B.x + B.y * B.y;
    p.x = scalar * B.x;
    p.y = scalar * B.y;
    return p;
}



float dot_product(Point A, Point B) {
    return A.x * B.x + A.y * B.y;
}


float max_in2(float a, float b) {
    return (a > b) ? a : b;
}

float min_in2(float a, float b) {
    return (a < b) ? a : b;
}


float max_in4(float a, float b, float c, float d) {
    float ab = max_in2(a, b);
    float cd = max_in2(c, d);
    return max_in2(ab, cd);
}


float min_in4(float a, float b, float c, float d) {
    float ab = min_in2(a, b);
    float cd = min_in2(c, d);
    return min_in2(ab, cd);
}
