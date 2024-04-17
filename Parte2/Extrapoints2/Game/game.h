#define xDim 240
#define yDim 320

typedef struct {						
	int leftx, rightx, topy, bottony, dirx, diry;
} Ball;

void game_init(int score_p1, int score_p2);
void changePadlle1Pos(int currPos);
void game_start(void);
void game_end(int winner);
void game_pause(void);
void game_resume(void);
void changePadlle2Pos(void);
void game_scoreUpdate(void);
void pointP1(void);
void pointP2(void);
