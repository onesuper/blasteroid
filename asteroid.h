
#define MAX_ASTEROID_NUM 4


typedef struct {
    float sx;
    float sy;
    float heading;
    float twist;
    float speed;
    float rot_velocity;
    float scale;
    int live;
    ALLEGRO_COLOR color;
} Asteroid;



void asteroids_init(Asteroid a[], int size);
void asteroids_appear(Asteroid a[], int size, int width, int height);
void asteroids_move(Asteroid a[], int size, int width, int height);
void asteroids_draw(Asteroid a[], int size);
