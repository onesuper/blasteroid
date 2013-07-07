
#define MAX_BLAST_NUM 20


typedef struct {
    float sx;
    float sy;
    float heading;
    float speed;
    int live;
    ALLEGRO_COLOR color;
} Blast;


void blasts_init(Blast b[], int size);
void blasts_draw(Blast b[], int size);
void blasts_fire(Blast b[], int size, Spaceship* s);
void blasts_move(Blast b[], int size, int width, int height);
