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
    drawText(screenX + 15, screenY, "  NEXT       ", 240);
    drawText(screenX + 15, screenY + 1, " ", 240);
    drawText(screenX + 15, screenY + 2, " ", 240);
    drawText(screenX + 15, screenY + 3, " ", 240);
    drawText(screenX + 15, screenY + 4, " ", 240);

    drawText(screenX + 21, screenY + 1, " ", 240);
    drawText(screenX + 21, screenY + 2, " ", 240);
    drawText(screenX + 21, screenY + 3, " ", 240);
    drawText(screenX + 21, screenY + 4, " ", 240);

    drawText(screenX + 15, screenY + 5, "  ", 240);
    drawText(screenX + 16, screenY + 5, "  ", 240);
    drawText(screenX + 17, screenY + 5, "  ", 240);
    drawText(screenX + 18, screenY + 5, "  ", 240);
    drawText(screenX + 19, screenY + 5, "  ", 240);
    drawText(screenX + 20, screenY + 5, "  ", 240);
    drawText(screenX + 21, screenY + 5, " ", 240);
}

void drawInfoTextUI() {
    // Draw info
    drawText(1, 20,  "PIECES", 7);
    drawText(1, 21, "0, 0.00/S", 7);
    drawText(1, 22, "LINES", 7);
    drawText( 1, 23, "0/40", 7);
    drawText(1, 24, "TIME", 7);
    drawText(1, 25, "0:13.233", 7);

}

void drawInfoPIECES(int count) {
    char text[20];
    sprintf(text, "%d", count);
    drawText(1, 21, text, 7);
}

void drawInfoLINES(int count, int maxCount) {
    char text[20];
    sprintf(text, "%d/%d", count, maxCount);
    drawText(1, 23, text, 7);
}

void drawInfoTIME(char* timeFormatted) {
    drawText(1, 25, timeFormatted, 7);
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

