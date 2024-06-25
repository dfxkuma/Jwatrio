#include <stdio.h>

#include "../render/text.h"
#include "../utils/color.h"
#include "draw.h"

void drawBlock(int x, int y, int color) {
    movePos(x, y);
    setColor(color);
    printf(" ");
    setColor(COLOR_DEFAULT);
}

void drawText(int x, int y, char* text, int color) {
    setColor(color);
    movePos(x, y);
    printf("%s", text);
    setColor(COLOR_DEFAULT);
}

void drawNextBlockUI(int screenX, int screenY) {

    drawText(screenX + 13, screenY, "  NEXT       ", 240);
    drawText(screenX + 13, screenY + 1, " ", 240);
    drawText(screenX + 13, screenY + 2, " ", 240);
    drawText(screenX + 13, screenY + 3, " ", 240);
    drawText(screenX + 13, screenY + 4, " ", 240);

    drawText(screenX + 19, screenY + 1, " ", 240);
    drawText(screenX + 19, screenY + 2, " ", 240);
    drawText(screenX + 19, screenY + 3, " ", 240);
    drawText(screenX + 19, screenY + 4, " ", 240);


}
void drawHOLDUI(int screenX, int screenY) {
    drawText(0, screenY, "  HOLD       ", 240);
    drawText(0, screenY + 1, " ", 240);
    drawText(0, screenY + 2, " ", 240);
    drawText(0, screenY + 3, " ", 240);
    drawText(0, screenY + 4, " ", 240);

    drawText(6, screenY + 1, " ", 240);
    drawText(6, screenY + 2, " ", 240);
    drawText(6, screenY + 3, " ", 240);
    drawText(6, screenY + 4, " ", 240);

    drawText(0, screenY + 5, "  ", 240);
    drawText(1, screenY + 5, "  ", 240);
    drawText(2, screenY + 5, "  ", 240);
    drawText(3, screenY + 5, "  ", 240);
    drawText(4, screenY + 5, "  ", 240);
    drawText(5, screenY + 5, "  ", 240);
    drawText(6, screenY + 5, " ", 240);
}

void drawGameUI(int screenX, int screenY) {
    setColor(7); // White color for grid
    for (int y = 0; y <= 21; y++) {
        for (int x = 2; x <= 12; x++) {
            movePos(screenX + x, screenY + y);
            if (x == 2 || x == 12) {
                printf("║"); // 테두리 문자 출력
            } else if (y == 0 ) {
                printf("▔");
            }else if (y == 21){
                printf("▂" );
            } else {
                printf(". ");
            }
        }
    }
}

void drawInfoTextUI() {
    drawText(1, 20,  "PIECES", 7);
    drawText(1, 21, "0, 0.00/S", 7);
    drawText(1, 22, "LINES", 7);
    drawText( 1, 23, "0/40", 7);
    drawText(1, 24, "TIME", 7);
    drawText(1, 25, "0:13.233", 7);


    drawText(18, 20,  "J W A T R I O", 7);
    drawText(18, 21, "Classic Game", 7);
    drawText(18, 25, "10 FPS [GOOD]", 7);

    drawText(10, 27,  "[ @hb072 ]", 7);

    for (int i = 2; i < 29; i++) {
        drawText(27, i, "║", 7);
    }
}

int TETRIS_BLOCK[7][4][4][4]={
        {{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
                {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
        {{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
                {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
        {{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},
                {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
        {{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},
                {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}},
        {{0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0},{0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},
                {0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}},
        {{0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},
                {0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},{0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}},
        {{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},
                {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}}
};

