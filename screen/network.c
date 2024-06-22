#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#include "../render/render.h"
#include "../screen.h"

const char* frames[] = { "-", "/", "\\"};
int numFrames = sizeof(frames) / sizeof(frames[0]);
int currentFrame = 0;
int delay = 100000;

void renderEmoji(int x, int y) {
    movePos(x, y); printf("%s", frames[currentFrame]);
    currentFrame = (currentFrame + 1) % numFrames;
    Sleep(500);
}

int screenStartNetwork(int screenX, int screenY) {
    system("cls");

    int menuItems = 3;
    char* menuText[] = {
            " + 새 방 만들기 \n",
            " <1> 좌호빈 (0:5575) [ 2 / 15 ]\n",
            " <2> 로컬서버 (0:5575) [ 3 / 15 ] \n",
    };

    ConsoleRender* render = createRender(5, 4, menuText, menuItems);
    movePos(screenX + 6, screenY + 1); printf("J W A T R I O"); Sleep(100);
    movePos(screenX + 4, screenY + 4); printf("< 네트워크에서 게임하기 >"); Sleep(100);
    movePos(screenX + 4, screenY + 5); printf("                                  ");
    movePos(screenX + 4, screenY + 5); printf(" 네트워크에서 방 찾는중 "); Sleep(100);
    render->renderMenu(render);

    while (1) {
        renderEmoji(screenX + 16, screenY + 5);
        int callbackCode = render->menuKeyDetect(render);
        if (callbackCode < 0) {
            break;
        }
    }

    int selectedMenu = render->getSelectedMenu(render);
    if (selectedMenu == -1) {
        screenStartHome(screenX, screenY);
    }
}