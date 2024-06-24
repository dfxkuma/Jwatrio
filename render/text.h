#define CONSOLE_SELECTED 240
#define CONSOLE_DEFAULT 15

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

void movePos(int x, int y);
void setColor(int color);
void setCursorType(int c);