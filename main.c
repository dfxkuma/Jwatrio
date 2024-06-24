#include "render/menu.h"
#include "screen.h"
#include "render/text.h"

int main() {
    int screenX = 5, screenY = 4;
    setCursorType(NOCURSOR);

    screenStartHome(screenX, screenY);
    return 0;
}