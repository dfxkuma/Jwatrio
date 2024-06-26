#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

#include "../render/menu.h"
#include "../screen.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 6974
#define BROADCAST_IP "255.255.255.255"
#define BUFFER_SIZE 1024
#define TIMEOUT 5 // 타임아웃 설정 (초)
#define MAX_SERVERS 100 // 최대 서버 수
#define INET_ADDRSTRLEN 16 // IPv4 주소 문자열 길이

typedef struct {
    char ip[INET_ADDRSTRLEN];
    char name[50];
    int playerCount;
} ServerState;

const char* frames[] = { "-", "/", "\\"};
int numFrames = sizeof(frames) / sizeof(frames[0]);
int currentFrame = 0;

void renderEmoji(int x, int y) {
    movePos(x, y); printf("%s", frames[currentFrame]);
    currentFrame = (currentFrame + 1) % numFrames;
    Sleep(200);
}

ServerState serverList[];
int server_count = 0;

void add_server_ip(const char* ip, const char* name, const char* playerCount) {
    for (int i = 0; i < server_count; i++) {
        if (strcmp(serverList[i].ip, ip) == 0) {
            return; // 이미 배열에 있는 IP는 추가하지 않음
        }
    }

    if (server_count < MAX_SERVERS) {
        strncpy(serverList[server_count].ip, ip, INET_ADDRSTRLEN);
        strncpy(serverList[server_count].name, name, INET_ADDRSTRLEN);
        serverList[server_count].playerCount = atoi(playerCount);
        server_count++;
    }
}

// 서버 IP 주소를 출력하는 함수
void print_server_ips(int screenX, int screenY) {
    system("cls");
    char* menuText[] = {
        " * 서버 검색하기 \n",
    };

    for (int i=0; i<server_count; i++) {
        char buffer[100];
        sprintf(buffer, "<%d> %s [ %d / 2 ] - %s\n", i + 1, serverList[i].name, serverList[i].playerCount, serverList[i].ip);
        menuText[i+1] = buffer;
    }
    int menuCount = sizeof(menuText) / sizeof(menuText[0]);

    MenuRender* render = createMenuRender(5, 4, menuText, menuCount+1);
    movePos(screenX + 6, screenY + 1); printf("J W A T R I O"); Sleep(100);
    movePos(screenX + 4, screenY + 4); printf("< 네트워크에서 게임하기 >"); Sleep(100);
    movePos(screenX + 4, screenY + 5); printf("                                  ");
    movePos(screenX + 4, screenY + 5); printf(" 네트워크에서 방 찾는중 "); Sleep(100);
    render->renderMenu(render);

    while (1) {
        renderEmoji(screenX + 16, screenY + 5);
        int callbackCode = render->menuKeyDetect(render);
        if (callbackCode < 0) {
            break;
        }
    }

    int selectedMenu = render->getSelectedMenu(render);
    if (selectedMenu == -1) {
        screenStartHome(screenX, screenY);
    }

    if (selectedMenu > 0) {
        printf("Selected: %s\n", serverList[selectedMenu - 1].ip);
    }

//    for (int i = 0; i < server_count; i++) {
//        printf("%s\n", serverList[i].ip);
//    }
}

int scanNetwork(int screenX, int screenY) {
    system("cls");

    WSADATA wsa;
    SOCKET sockfd;
    struct sockaddr_in broadcast_addr, from_addr;
    int from_addr_len = sizeof(from_addr);
    char buffer[BUFFER_SIZE];
    fd_set readfds;
    struct timeval tv;

    // 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code : %d\n", WSAGetLastError());
        return 1;
    }

    // 소켓 생성
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // 브로드캐스트 사용 설정
    BOOL broadcast_enable = TRUE;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast_enable, sizeof(broadcast_enable)) == SOCKET_ERROR) {
        printf("setsockopt(SO_BROADCAST) failed with error code : %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // 브로드캐스트 주소 설정
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);
    broadcast_addr.sin_port = htons(PORT);

    while (1) {

        // 메시지 전송
        const char *message = "jtr: udp broadcast check";
        if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) == SOCKET_ERROR) {
            printf("sendto() failed with error code : %d\n", WSAGetLastError());
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }

        // 타임아웃 설정
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;

        // 서버 응답 수신
        while (1) {
            print_server_ips(screenX, screenY);
            FD_ZERO(&readfds);
            FD_SET(sockfd, &readfds);

            int activity = select(0, &readfds, NULL, NULL, &tv);

            if (activity == SOCKET_ERROR) {
                printf("select() failed with error code : %d\n", WSAGetLastError());
                break;
            } else if (activity == 0) {
                break;
            }

            if (FD_ISSET(sockfd, &readfds)) {
                int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&from_addr, &from_addr_len);
                if (recv_len == SOCKET_ERROR) {
                    printf("recvfrom() failed with error code : %d\n", WSAGetLastError());
                    break;
                }

                buffer[recv_len] = '\0';

                // 올바른 응답 확인
                if (strncmp(buffer, "jtr: check response from server|", strlen("jtr: check response from server|")) == 0) {
                    char *response_start = buffer + strlen("jtr: check response from server|");

                    char *name_part = strtok(response_start, ",");
                    char *port_part = strtok(NULL, ",");

                    if (name_part != NULL && port_part != NULL) {
                        const char* server_ip = inet_ntoa(from_addr.sin_addr);

                        add_server_ip(server_ip, name_part, port_part);
                        print_server_ips(screenX, screenY);
                    }
                }
            }
        }
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}

int screenStartNetwork(int screenX, int screenY) {
    while (1) {
        scanNetwork(screenX, screenY);
    }
}