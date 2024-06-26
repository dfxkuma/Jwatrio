#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "event.h"

#pragma comment(lib, "ws2_32.lib")
#define BUFFER_SIZE 2048

SOCKET server_fd;

DWORD WINAPI receiveGameState() {
    char buffer[BUFFER_SIZE];

    while (1) {
        int valread = recv(server_fd, buffer, BUFFER_SIZE, 0);
        if (valread > 0) {
            buffer[valread] = '\0';
            printf("Server: %s\n", buffer);
        } else if (valread == 0) {
            printf("Server disconnected.\n");
            break;
        } else {
            printf("recv failed: %d\n", WSAGetLastError());
            break;
        }
    }
}

void handle_server(char* ip, char* username) {
    WSADATA wsa;
    struct sockaddr_in server_address;
    HANDLE recv_thread;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return;
    }

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6974);

    if (InetPton(AF_INET, ip, &server_address.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported\n");
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    if (connect(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("Connection failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    recv_thread = CreateThread(NULL, 0, receiveGameState, NULL, 0, NULL);
    if (recv_thread == NULL) {
        printf("Error creating receive thread: %d\n", GetLastError());
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    char message[1024];
    printf_s("jtr: set name: %s", username);
    send(server_fd, message, strlen(message), 0);

    // Wait for the receiving thread to finish
    WaitForSingleObject(recv_thread, INFINITE);
    CloseHandle(recv_thread);

    // Cleanup
    closesocket(server_fd);
    WSACleanup();
}

int onGameBlockChange(int x, int y, int blockType, int blockStatus) {
};

int onGamePause(int status);

int onGameReady() {

};

int onGameEngineStart(char* username, char* ip) {
    handle_server(ip, username);
}

int onGameLineCrash(int line);

int onGameOver();

int onGameScoreChanged(int score);
