#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define CONSOLE_SELECTED 240
#define CONSOLE_DEFAULT 15

typedef struct {
    int key;
    int menu;
    int x;
    int y;
    int renderCheck;
    int menuItems;
    char** menuSelectText;
    void (*movePos)(int x, int y);
    void (*setColor)(int color);
    void (*renderMenu)();
    int (*menuKeyDetect)();
    int (*getSelectedMenu)();
} ConsoleRender;

ConsoleRender* createRender(int x, int y, char** menuSelectText, int menuItems);
void movePos(int x, int y);
void setColor(int color);
void renderMenu(ConsoleRender* console);
int menuKeyDetect(ConsoleRender* console);
int getSelectedMenu(ConsoleRender* console);