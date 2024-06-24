#include <windows.h>

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

void movePos(int x, int y) {
    COORD pos = { 2 * x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void setCursorType(CURSOR_TYPE c){ //커서숨기는 함수
    CONSOLE_CURSOR_INFO CurInfo;

    switch (c) {
        case NOCURSOR:
            CurInfo.dwSize=1;
            CurInfo.bVisible=FALSE;
            break;
        case SOLIDCURSOR:
            CurInfo.dwSize=100;
            CurInfo.bVisible=TRUE;
            break;
        case NORMALCURSOR:
            CurInfo.dwSize=20;
            CurInfo.bVisible=TRUE;
            break;
    }
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);
}