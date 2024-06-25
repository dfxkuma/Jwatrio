#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#include "../render/menu.h"
#include "../screen.h"
#include "../render/prompt.h"
#include "../game/engine.h"

int screenStartSoloGame(int screenX, int screenY) {
    system("cls");

    char *nickname = prompt(5, 5, "좌트리오에서 사용할 닉네임을 입력하세요 (20자 제한)", 20);
    printf("\n\n입력한 문자열: %s\n", nickname);

    system("cls");
    int menuItems = 3;
    char* menuText[] = {
            " 40L  | 40 LINES - QUICKLY COMPLETE 40 LINES \n",
            " BLZ  | BLITZ - A TWO MINUTE RACE            \n",
            " ZEN  | ZEN - RELAX OR TRAIN MODE            \n",
    };

    MenuRender* render = createMenuRender(3, 4, menuText, menuItems);
    movePos(screenX + 10, screenY + 5); printf("SOLO | %s", nickname); Sleep(100);
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
        case -1:
            screenStartHome(screenX, screenY);
            break;
        case 0:
            break;
        case 1:
            screenStartSoloGame(screenX, screenY);
            break;
        case 2:
            startGameEngine(screenX, screenY);
    }
}