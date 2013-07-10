

#include <math.h>
#include "utils.h"




void point_rotate(Point* p, Point center, float theta) {
    
    float x = p->x;
    float y = p->y;

    p->x = center.x + (x - center.x) * cos(theta) + (y - center.y) * sin(theta);
    p->y = center.y - (x - center.x) * sin(theta) + (y - center.y) * cos(theta);

}
