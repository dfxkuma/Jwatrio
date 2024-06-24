#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "../render/menu.h"
#include "../screen.h"
#include "../render/prompt.h"

#pragma comment(lib, "ws2_32.lib")

SOCKET server_fd;

int screenStartConnectServer(int screenX, int screenY) {
    system("cls");

    movePos(screenX + 3, screenY + 5); printf("Connecting to JWATRIO Server..."); Sleep(100);
    movePos(screenX + 0, screenY + 12); printf("region: ap-aws-seoul"); Sleep(100);
    movePos(screenX + 9, screenY + 2); printf("WELCOME TO JWATRIO!"); Sleep(100);

    WSADATA wsa;
    struct sockaddr_in address;
    DWORD server_thread_id;

    // 서버 스레드 시작
    int port = 8080;
    HANDLE server_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) '127.0.0.1', &port, 0, &server_thread_id);
    if (server_thread == NULL) {
        printf("Error creating server thread: %d\n", GetLastError());
        return 1;
    }

    // 클라이언트 초기화
    WSAStartup(MAKEWORD(2, 2), &wsa);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);  // 서버와 동일한 포트 번호 사용
    InetPton(AF_INET, "127.0.0.1", &address.sin_addr);

    if (connect(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Connection failed: %d\n", WSAGetLastError());
        return 1;
    }

}