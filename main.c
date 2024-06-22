#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#include "render/render.h"
#include "screen.h"

int main() {
    int menuItems = 5;
    int screenX = 5, screenY = 4;
    char* menuText[] = {
            " 1-Player Game \n",
            " Network Game \n",
            " Options \n",
            " Highscores \n",
            " 좌호빈사진보기 \n",
    };

    ConsoleRender* render = createRender(5, 4, menuText, menuItems);
    movePos(screenX, screenY + 5); printf("      Yuchan Han, Seongju Cho"); Sleep(100);
    movePos(screenX + 6, screenY + 2); printf("J W A T R I O"); Sleep(100);
    render->start(render);

    int selectedMenu = render->getSelectedMenu(render);
    switch (selectedMenu) {
        case 0:
            printf("1-Player Game\n");
            break;
        case 1:
            screenStartNetwork(screenX, screenY);
            break;
        case 2:
            printf("Options\n");
            break;
        case 3:
            printf("Highscores\n");
            break;
        case 4:
            printf("좌호빈사진보기\n");
            break;
    }

    // 메모리 해제
    for (int i = 0; i < menuItems; i++) {
        free(render->menuSelectText[i]);
    }
    free(render->menuSelectText);
    free(render);


    return 0;
}