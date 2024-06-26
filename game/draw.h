#define BLOCK_ACTIVE -2
#define BLOCK_CEILLING -1
#define BLOCK_EMPTY 0
#define BLOCK_WALL 1
#define BLOCK_INACTIVE 2

void drawBlock(int x, int y, int color);
void drawText(int x, int y, char* text, int color);
void drawHOLDUI(int screenX, int screenY);
void drawGameUI(int screenX, int screenY);
void drawInfoTextUI(int linesMaxCount);
void drawNextBlockUI(int screenX, int screenY);

void drawInfoPIECES(int count);
void drawInfoLINES(int count, int maxCount);
void drawInfoTIME(char* timeFormatted);

extern int TETRIS_BLOCK[7][4][4][4];

