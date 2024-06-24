#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#include "../render/menu.h"
#include "../screen.h"
#include "../render/prompt.h"

int screenStartConnectServer(int screenX, int screenY) {
    system("cls");

    movePos(screenX + 3, screenY + 5); printf("Connecting to JWATRIO Server..."); Sleep(100);
    movePos(screenX + 0, screenY + 12); printf("region: ap-aws-seoul"); Sleep(100);
    movePos(screenX + 9, screenY + 2); printf("WELCOME TO JWATRIO!"); Sleep(100);
}