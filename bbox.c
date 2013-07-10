
#include <allegro5/allegro_primitives.h>
#include "bbox.h"




int axis_overlap(Point A_LT, Point A_RT, Point A_LB, Point A_RB,
                 Point B_LT, Point B_RT, Point B_LB, Point B_RB, Point axis) {

    /***************************************************************************************
     *          Determine overlap alongside Axis1
     ***************************************************************************************/

    /* Project 6 corners onto the Axis1 */
    Point proj_A_LT_Axis = project(A_LT, axis);
    Point proj_A_RT_Axis = project(A_RT, axis);
    Point proj_A_LB_Axis = project(A_LB, axis);
    Point proj_A_RB_Axis = project(A_RB, axis);

    Point proj_B_LT_Axis = project(B_LT, axis);
    Point proj_B_RT_Axis = project(B_RT, axis);
    Point proj_B_LB_Axis = project(B_LB, axis);
    Point proj_B_RB_Axis = project(B_RB, axis);

    float a1 = dot_product(proj_A_LT_Axis, axis);
    float a2 = dot_product(proj_A_RT_Axis, axis);
    float a3 = dot_product(proj_A_LB_Axis, axis);    
    float a4 = dot_product(proj_A_RB_Axis, axis);

    float b1 = dot_product(proj_B_LT_Axis, axis);
    float b2 = dot_product(proj_B_RT_Axis, axis);
    float b3 = dot_product(proj_B_LB_Axis, axis);
    float b4 = dot_product(proj_B_RB_Axis, axis);

    
    float max_A = max_in4(a1, a2, a3, a4);
    float min_A = min_in4(a1, a2, a3, a4);
    float max_B = max_in4(b1, b2, b3, b4);
    float min_B = min_in4(b1, b2, b3, b4);

    if (min_B > max_A || min_A > max_B) 
        return 0;      /* None Overlap */
    else 
        return 1;

}

int bbox_overlap(Bbox A, Bbox B) {

    /* This method is described at http://www.gamedev.net/page/resources/_/technical/game-programming/2d-rotated-rectangle-collision-r2604 */

    Point Axis1, Axis2, Axis3, Axis4;
    Point A_LT, A_LB, A_RT, A_RB;
    Point B_LT, B_LB, B_RT, B_RB;


    /* Mapping A's corners */
    A_LT.x = A.center.x - A.left;
    A_LT.y = A.center.y - A.top;   
    A_LB.x = A.center.x - A.left;
    A_LB.y = A.center.y + A.bottom;
    A_RT.x = A.center.x + A.right;
    A_RT.y = A.center.y - A.top;   
    A_RB.x = A.center.x + A.right;
    A_RB.y = A.center.y + A.bottom;
    

    /* Rotate the corners */
    point_rotate(&A_LT, A.center, A.heading);
    point_rotate(&A_LB, A.center, A.heading);
    point_rotate(&A_RT, A.center, A.heading);
    point_rotate(&A_RB, A.center, A.heading);


    /* Axis 1,2 for A */
    Axis1.x = A_RT.x - A_LT.x;
    Axis1.y = A_RT.y - A_LT.y;
    Axis2.x = A_RT.x - A_RB.x;
    Axis2.y = A_RT.y - A_RB.y;
    

    /* Mapping B's corners */
    B_LT.x = B.center.x - B.left;
    B_LT.y = B.center.y - B.top;   
    B_LB.x = B.center.x - B.left;
    B_LB.y = B.center.y + B.bottom;
    B_RT.x = B.center.x + B.right;
    B_RT.y = B.center.y - B.top;   
    B_RB.x = B.center.x + B.right;
    B_RB.y = B.center.y + B.bottom;


    /* rotate the corners */
    point_rotate(&B_LT, B.center, B.heading);
    point_rotate(&B_LB, B.center, B.heading);
    point_rotate(&B_RT, B.center, B.heading);
    point_rotate(&B_RB, B.center, B.heading);
    

    /* Axis 3,4 for B */
    Axis3.x = B_LT.x - B_LB.x;
    Axis3.y = B_LT.y - B_LB.y;
    Axis4.x = B_LT.x - B_RT.x;
    Axis4.y = B_LT.y - B_RT.y;


    int r1, r2, r3, r4 = 0;

    r1 = axis_overlap(A_LT, A_RT, A_LB, A_RB, B_LT, B_RT, B_LB, B_RB, Axis1);
    r2 = axis_overlap(A_LT, A_RT, A_LB, A_RB, B_LT, B_RT, B_LB, B_RB, Axis2);
    r3 = axis_overlap(A_LT, A_RT, A_LB, A_RB, B_LT, B_RT, B_LB, B_RB, Axis3);
    r4 = axis_overlap(A_LT, A_RT, A_LB, A_RB, B_LT, B_RT, B_LB, B_RB, Axis4);

    return r1 && r2 && r3 && r4;
}





void bbox_draw(Bbox b) {
        
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_rotate_transform(&transform, b.heading);
    al_translate_transform(&transform, b.center.x, b.center.y);
    al_use_transform(&transform);
                
    al_draw_line( -b.left,    -b.top,  -b.left,  b.bottom, al_map_rgb(255, 0, 0), 1.0f);
    al_draw_line( -b.left,    -b.top,  b.right,   -b.top, al_map_rgb(255, 0, 0), 1.0f);
    al_draw_line( -b.left,  b.bottom,  b.right, b.bottom, al_map_rgb(255, 0, 0), 1.0f);
    al_draw_line( b.right,    -b.top,  b.right, b.bottom, al_map_rgb(255, 0, 0), 1.0f);
    
}
