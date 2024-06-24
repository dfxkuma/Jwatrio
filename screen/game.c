#include <stdio.h>
#include <windows.h>

#include "../render/text.h"

void drawBlock(int x, int y, int color) {
    movePos(x, y);
    setColor(color);
    printf("  ");
    setColor(CONSOLE_DEFAULT);
}

void drawText(int x, int y, char* text, int color) {
    setColor(color);
    movePos(x, y);
    printf("%s", text);
}

int screenStartGame(int screenX, int screenY) {
    system("cls"); // Clear the console

    // Draw grid
    setColor(7); // White color for grid
    for (int y = 0; y <= 21; y++) {
        for (int x = 2; x <= 11; x++) {
            movePos(screenX + x, screenY + y);
            if (x == 2 || x == 11) {
                printf("║"); // 테두리 문자 출력
            } else if (y == 0 ) {
                printf("▔");
            }else if (y == 21){
                printf("▂" );
            } else {
                printf(".");
            }
        }
    }

    // Draw hold piece
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

    setColor(8);
    drawBlock(2, screenY + 2, 42);
    drawBlock(3, screenY + 2, 42);
    drawBlock(3, screenY + 3, 42);
    drawBlock(4, screenY + 3, 42);

    // Draw next pieces
    drawText(screenX + 13, screenY, "NEXT", 7);
    drawBlock(screenX + 13, screenY + 1, 14); // Yellow
    drawBlock(screenX + 14, screenY + 2, 14);
    drawBlock(screenX + 13, screenY + 3, 14);
    drawBlock(screenX + 14, screenY + 4, 14);

    drawText(screenX + 13, screenY, "  NEXT       ", 240);
    drawText(screenX + 13, screenY + 1, " ", 240);
    drawText(screenX + 13, screenY + 2, " ", 240);
    drawText(screenX + 13, screenY + 3, " ", 240);
    drawText(screenX + 13, screenY + 4, " ", 240);


    // Draw info
    drawText(1, 20,  "PIECES", 7);
    drawText(1, 21, "0, 0.00/S", 7);
    drawText(1, 22, "LINES", 7);
    drawText( 1, 23, "0/40", 7);
    drawText(1, 24, "TIME", 7);
    drawText(1, 25, "0:13.233", 7);

    // Prevent the console from closing immediately
    getchar();

    return 0;
}
