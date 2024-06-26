#include<stdlib.h>

#include "engine.h"
#include "../render/prompt.h"

void playNetworkGame(int screenX, int screenY, char* ip) {
    system("cls");
    char *username = prompt(screenX, screenY, "좌트리오에서 사용할 닉네임을 입력하세요 (20자 제한)", 20);

    startNetworkGameEngine(screenX, screenY, username, ip);
}

void playSolo40Lines(int screenX, int screenY) {

}

void playSoloZen(int screenX, int screenY) {

}

void playSoloBlitz(int screenX, int screenY) {

}