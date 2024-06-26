#include<stdlib.h>

#include "engine.h"

void playNetworkGame(int screenX, int screenY, const char* ip, const char* username) {
    system("cls");
    MultiGameSetting setting = {*ip, *username};
    startNetworkGameEngine(screenX, screenY, setting);

}

void playSolo40Lines(int screenX, int screenY) {

}

void playSoloZen(int screenX, int screenY) {

}

void playSoloBlitz(int screenX, int screenY) {

}