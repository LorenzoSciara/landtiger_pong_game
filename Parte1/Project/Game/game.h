#define xDim 240
#define yDim 320

typedef struct {						
	int leftx, rightx, topy, bottony, dirx, diry;
} Ball;

void game_init(void);
void changePadllePos(int currPos);
void game_start(void);
void game_end(void);
void game_pause(void);
void game_resume(void);