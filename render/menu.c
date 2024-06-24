#include "menu.h"
#include "text.h"
#include "color.h"

MenuRender* createMenuRender(int x, int y, char** menuSelectText, int menuItems) {
    MenuRender* console = (MenuRender*)malloc(sizeof(MenuRender));

    if (console != NULL) {
        console->key = 0;
        console->menu = 0;
        console->x = x;
        console->y = y;
        console->renderCheck = 0;
        console->menuItems = menuItems;
        console->menuSelectText = (char**)malloc(menuItems * sizeof(char*));
        for (int i = 0; i < menuItems; i++) {
            console->menuSelectText[i] = (char*)malloc(500 * sizeof(char));
            strncpy_s(console->menuSelectText[i], 500, menuSelectText[i], 499);
            console->menuSelectText[i][499] = '\0';  // Null-terminate
        }

        console->movePos = movePos;
        console->setColor = setColor;
        console->renderMenu = renderMenu;
        console->menuKeyDetect = menuKeyDetect;
        console->getSelectedMenu = getSelectedMenu;
    }
    return console;
}

void renderMenu(MenuRender* console) {
    int selected = 0;
    for (int yPos = 7; yPos < 7 + console->menuItems; yPos++) {
        if (console->menu + 7 == yPos) {
            // 미리 선택된 메뉴를 출력
            console->setColor(COLOR_BG_DEFAULT);
            selected = 1;
        }
        console->movePos(console->x + 5, console->y + yPos); printf("%s", console->menuSelectText[yPos - 7]);
        if (selected) {
            console->setColor(COLOR_DEFAULT);
            selected = 0;
        }
    }
}

int menuKeyDetect(MenuRender* console) {
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 27) { // ESC 키
            console->renderCheck = 1;
            console->menu = -1; // 뒤로가기
            return -1;
        }
        else if (ch == 0 || ch == 224) {
            ch = _getch(); // 방향키 입력
            if (ch == 72) { // 위쪽 화살표
                if (console->menu > 0) {
                    console->menu--;
                }
                console->renderMenu(console);
            }
            else if (ch == 80) { // 아래쪽 화살표
                if (console->menu < console->menuItems - 1) {
                    console->menu++;
                }
                console->renderMenu(console);
            }
        }
        else if (ch == 13) { // Enter 키
            console->renderCheck = 1; // 메뉴 선택
            return -1;
        }

    }
    return 0;
}

int getSelectedMenu(MenuRender* console) {
    return console->menu;
}