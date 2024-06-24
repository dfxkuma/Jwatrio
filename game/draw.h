void drawBlock(int x, int y, int color);
void drawText(int x, int y, char* text, int color);

typedef struct point
{
    int x, y;
} point;

extern int TETRIS_BLOCK[7][4][4][4];