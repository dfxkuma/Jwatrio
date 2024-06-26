#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#define BUFFER_SIZE 2048

extern SOCKET server_fd;

DWORD WINAPI receiveGameState();
void handle_server(char* ip, char* username);
int onGameBlockChange(int x, int y, int blockType, int blockStatus);
int onGamePause(int status);
int onGameReady();
int onGameLineCrash(int line);
int onGameOver();
int onGameScoreChanged(int score);
int onGameEngineStart(char* username, char* ip);
