

#define MAX_SHIP_SPEED 2.5
#define SHIP_SPEEDUP 0.08
#define SHIP_ROTATE_ANGLE 0.05

typedef struct {
    float sx;
    float sy;
    float heading;
    float speed;
    int live;          // Live or dead?
    int accelerating;  // Is accelerating ?
    ALLEGRO_COLOR color;
} Spaceship;


void ship_init(Spaceship* s, int x, int y);
void ship_draw(Spaceship* s);
void ship_move(Spaceship* s, int width, int height);
void ship_acc(Spaceship* s);
void ship_slowndown(Spaceship* s);
void ship_turn_right(Spaceship* s);
void ship_turn_left(Spaceship* s);
