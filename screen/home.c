#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#include "../render/render.h"
#include "../screen.h"

int screenStartHome(int screenX, int screenY) {
    system("cls");

    int menuItems = 4;
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
    render->renderMenu(render);
    while (1) {
        int callbackCode = render->menuKeyDetect(render);
        if (callbackCode < 0) {
            break;
        }
    }

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
}