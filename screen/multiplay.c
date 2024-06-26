#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#include "../render/menu.h"
#include "../screen.h"
#include "../render/prompt.h"
#include "../game/engine.h"

int screenStartMultiplay(int screenX, int screenY, char* ip, char* roomName, int playerCount) {
    system("cls");

    movePos(screenX + 6, screenY + 1); printf("J W A T R I O"); Sleep(100);
    movePos(screenX + 4, screenY + 4); printf("< %s (%s)에 연결중... >                                 ", roomName, ip); Sleep(100);
    movePos(screenX + 4, screenY + 5); printf("                                  ");

    if (playerCount == 2) {
        movePos(screenX + 4, screenY + 4); printf("< 방에 참가할 수 없음 >      "); Sleep(100);
        movePos(screenX + 3, screenY + 6); printf("모집 인원이 모두 찼습니다. 5초 뒤에 홈으로 이동합니다..."); Sleep(100);
    } else {
        movePos(screenX + 4, screenY + 4); printf("< 방에 참가할 수 없음 >                "); Sleep(100);
        movePos(screenX + 3, screenY + 6); printf("서버가 응답이 없습니다."); Sleep(100);
    }
}