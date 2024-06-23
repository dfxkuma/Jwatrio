#include <stdio.h>
#include <windows.h>

#include "../render/text.h"

void drawBlock(int x, int y, int color) {
    setColor(240);
    movePos(x, y);
    printf("  ");
    setColor(color);
}

void drawText(int x, int y, char* text, int color) {
    setColor(color);
    movePos(x, y);
    printf("%s", text);
}

int screenStartGame(int screenX, int screenY) {
    system("cls"); // Clear the console

    // Draw grid
    char borderChar = '#';
    setColor(7); // White color for grid
    for (int y = 0; y <= 21; y++) {
        for (int x = 2; x <= 11; x++) {
            movePos(screenX + x, screenY + y);
            if (y == 0 || y == 21 || x == 2 || x == 11) {
                printf("%c", borderChar); // 테두리 문자 출력
            } else {
                printf(".");
            }
        }
    }

    // Draw hold piece
    drawText(1, screenY, "HOLD", 7);
    drawBlock(1, screenY + 1, 8); // Dark grey
    drawBlock(1, screenY + 2, 8);
    drawBlock(2, screenY + 3, 8);
    drawBlock(2, screenY + 4, 8);

    // Draw next pieces
    drawText(screenX + 13, screenY, "NEXT", 7);
    drawBlock(screenX + 13, screenY + 1, 14); // Yellow
    drawBlock(screenX + 14, screenY + 2, 14);
    drawBlock(screenX + 13, screenY + 3, 14);
    drawBlock(screenX + 14, screenY + 4, 14);


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
