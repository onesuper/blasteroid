
#include <allegro5/allegro_primitives.h>
#include "bbox.h"

#if 0
int bbox_collide(Bbox b1, Bbox b2) {

    if ((b1.origin.x > b2.origin.x + b2.width -1) ||     /* 1 is on the right side of 2 */
        (b1.origin.y > b2.origin.y + b2.height -1) ||    /* 1 is under 2 */
        (b2.origin.x > b1.origin.x + b1.width -1) ||
        (b2.origin.y > b1.origin.y + b1.height -1)) {
    
        /* no collision */
        return 0;
    }

    /* collision */
    return 1;
}

#endif

void bbox_draw(Bbox b) {
    
    
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_rotate_transform(&transform, b.heading);
    al_translate_transform(&transform, b.center.x, b.center.y);
    al_use_transform(&transform);
    
            
    al_draw_line( -b.left,    -b.top,  -b.left,  b.bottom, al_map_rgb(200, 200, 200), 1.0f);
    al_draw_line( -b.left,    -b.top,  b.right,   -b.top, al_map_rgb(200, 200, 200), 1.0f);
    al_draw_line( -b.left,  b.bottom,  b.right, b.bottom, al_map_rgb(200, 200, 200), 1.0f);
    al_draw_line( b.right,    -b.top,  b.right, b.bottom, al_map_rgb(200, 200, 200), 1.0f);
    
}
