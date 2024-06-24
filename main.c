#include "render/menu.h"
#include "screen.h"
#include "render/text.h"
#include "server/server.h"

int main() {
    int screenX = 5, screenY = 4;
    setCursorType(NOCURSOR);

//    startGameServer();

    screenStartHome(screenX, screenY);

    return 0;
}