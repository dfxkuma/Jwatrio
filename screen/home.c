#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#include "../render/menu.h"
#include "../screen.h"
#include "../render/prompt.h"
#include "../game/engine.h"

int screenStartHome(int screenX, int screenY) {
    system("cls");

    char *menuText[] = {
            " MP    | MULTIPLAY - PLAY ONLINE WITH FRIENDS \n",
            " 1P    | SOLO - CHALLENGE YOURSELF            \n",
            " CFG   | CONFIG - TWEAK YOUR JWATRIO          \n",
            " ABOUT | ALL ABOUT JWATRIO                    \n",
    };
    int menuCount = sizeof(menuText) / sizeof(menuText[0]);

    MenuRender* render = createMenuRender(3, 4, menuText, menuCount);
    movePos(screenX + 12, screenY + 5); printf("HOME"); Sleep(100);
    movePos(screenX + 9, screenY + 2); printf("WELCOME TO JWATRIO!"); Sleep(100);
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
            screenStartNetwork(screenX, screenY);
            break;
        case 1:
            screenStartSoloGame(screenX, screenY);
            break;
        case 2:
            printf("Config");
            break;
        case 3:
            printf("About");
            break;
        case 4:
    }
}