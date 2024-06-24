#include <stdio.h>
#include <windows.h>

#include "../render/text.h"
#include "../utils/color.h"
#include "../game/draw.h"

void renderHOLDUI (int screenX, int screenY) {
    drawText(0, screenY, "  HOLD       ", COLOR_BG_DEFAULT);
    drawText(0, screenY + 1, " ", COLOR_BG_DEFAULT);
    drawText(0, screenY + 2, " ", COLOR_BG_DEFAULT);
    drawText(0, screenY + 3, " ", COLOR_BG_DEFAULT);
    drawText(0, screenY + 4, " ", COLOR_BG_DEFAULT);

    drawText(6, screenY + 1, " ", COLOR_BG_DEFAULT);
    drawText(6, screenY + 2, " ", COLOR_BG_DEFAULT);
    drawText(6, screenY + 3, " ", COLOR_BG_DEFAULT);
    drawText(6, screenY + 4, " ", COLOR_BG_DEFAULT);

    drawText(0, screenY + 5, "  ", COLOR_BG_DEFAULT);
    drawText(1, screenY + 5, "  ", COLOR_BG_DEFAULT);
    drawText(2, screenY + 5, "  ", COLOR_BG_DEFAULT);
    drawText(3, screenY + 5, "  ", COLOR_BG_DEFAULT);
    drawText(4, screenY + 5, "  ", COLOR_BG_DEFAULT);
    drawText(5, screenY + 5, "  ", COLOR_BG_DEFAULT);
    drawText(6, screenY + 5, " ", COLOR_BG_DEFAULT);
}
void renderUI (int screenX, int screenY) {
    renderHOLDUI(screenX, screenY);
}