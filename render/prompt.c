#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "text.h"

char* prompt(int x, int y, char* message, int size) {
    movePos(x + 6, y + 4); printf("[?] %s", message); Sleep(100);
    movePos(x + 6, y + 5); printf(" > "); Sleep(100);
    movePos(x + 8, y + 6); printf("ㄴ [Enter]를 눌러주세요.");
    movePos(x + 8, y + 5);
    char* answer = (char*)malloc(size);
    if (fgets(answer, size, stdin) != NULL) {
        size_t len = strlen(answer);
        if (len > 0 && answer[len - 1] == '\n') {
            answer[len - 1] = '\0';
        }
    }
    return answer;
}