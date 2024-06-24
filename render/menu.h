#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

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
} MenuRender;

MenuRender* createMenuRender(int x, int y, char** menuSelectText, int menuItems);
void movePos(int x, int y);
void setColor(int color);
void renderMenu(MenuRender* console);
int menuKeyDetect(MenuRender* console);
int getSelectedMenu(MenuRender* console);