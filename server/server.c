#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define MAX_SESSIONS 10
#define MAX_PLAYERS 2

typedef struct {
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    int currentPiece;
    int x, y;
    int score;
    char username[50];  // 사용자 이름 추가
} GameState;

typedef struct {
    GameState players[MAX_PLAYERS];
    int playerCount;
} GameSession;

GameSession gameSessions[MAX_SESSIONS];
int sessionCount = 0;
SOCKET client_fds[MAX_SESSIONS * MAX_PLAYERS];
HANDLE client_threads[MAX_SESSIONS * MAX_PLAYERS];
CRITICAL_SECTION session_mutex;

void initGameSession(GameSession *session) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        memset(&session->players[i], 0, sizeof(GameState));
        session->players[i].x = BOARD_WIDTH / 2;
        session->players[i].y = 0;
        session->players[i].currentPiece = 1;  // 예제 조각 ID
    }
    session->playerCount = 0;
}

int createGameSession() {
    EnterCriticalSection(&session_mutex);
    if (sessionCount >= MAX_SESSIONS) {
        LeaveCriticalSection(&session_mutex);
        return -1;
    }
    initGameSession(&gameSessions[sessionCount]);
    sessionCount++;
    LeaveCriticalSection(&session_mutex);
    return sessionCount - 1;
}

int joinGameSession(int sessionId, SOCKET client_fd, const char *username) {
    EnterCriticalSection(&session_mutex);
    if (sessionId >= sessionCount || gameSessions[sessionId].playerCount >= MAX_PLAYERS) {
        LeaveCriticalSection(&session_mutex);
        return -1;
    }
    int playerId = gameSessions[sessionId].playerCount;
    gameSessions[sessionId].playerCount++;
    strncpy(gameSessions[sessionId].players[playerId].username, username, sizeof(gameSessions[sessionId].players[playerId].username) - 1);
    client_fds[sessionId * MAX_PLAYERS + playerId] = client_fd;
    LeaveCriticalSection(&session_mutex);
    return playerId;
}

void sendGameStateToClients(int sessionId) {
    char buffer[2048];
    GameSession *session = &gameSessions[sessionId];

    // 간단한 문자열 직렬화로 상태 전송
    sprintf(buffer, "UPDATE: P1 (%s) - x: %d, y: %d, P2 (%s) - x: %d, y: %d",
            session->players[0].username, session->players[0].x, session->players[0].y,
            session->players[1].username, session->players[1].x, session->players[1].y);

    for (int i = 0; i < session->playerCount; i++) {
        send(client_fds[sessionId * MAX_PLAYERS + i], buffer, strlen(buffer), 0);
    }
}

void processClientRequest(SOCKET client_fd, int sessionId, int playerId, char *request) {
    GameSession *session = &gameSessions[sessionId];
    GameState *state = &session->players[playerId];

    if (strcmp(request, "MOVE_LEFT") == 0) {
        if (state->x > 0) state->x--;
    } else if (strcmp(request, "MOVE_RIGHT") == 0) {
        if (state->x < BOARD_WIDTH - 1) state->x++;
    } else if (strcmp(request, "ROTATE") == 0) {
        state->currentPiece = (state->currentPiece % 4) + 1;
    } else if (strcmp(request, "DROP") == 0) {
        if (state->y < BOARD_HEIGHT - 1) state->y++;
    }

    sendGameStateToClients(sessionId);
}


DWORD WINAPI handleClient(LPVOID arg) {
    SOCKET client_fd = *(SOCKET *)arg;
    char buffer[1024] = {0};
    int sessionId = -1;
    int playerId = -1;
    char username[50] = {0};

    // 세션 참여 요청 처리
    int valread = recv(client_fd, buffer, 1024, 0);
    if (valread > 0) {
        if (strncmp(buffer, "CREATE_SESSION ", 15) == 0) {
            sscanf(buffer + 15, "%s", username);
            sessionId = createGameSession();
            if (sessionId >= 0) {
                playerId = joinGameSession(sessionId, client_fd, username);
                char response[50];
                sprintf(response, "SESSION_CREATED %d", sessionId);
                send(client_fd, response, strlen(response), 0);
            } else {
                send(client_fd, "CREATE_SESSION_FAILED", 21, 0);
            }
        } else if (strncmp(buffer, "JOIN_SESSION ", 12) == 0) {
            int reqSessionId;
            sscanf(buffer + 12, "%d %s", &reqSessionId, username);
            sessionId = reqSessionId;
            playerId = joinGameSession(sessionId, client_fd, username);
            if (playerId >= 0) {
                send(client_fd, "JOIN_SESSION_SUCCESS", 20, 0);
            } else {
                send(client_fd, "JOIN_SESSION_FAILED", 19, 0);
            }
        }
    }

    while (valread > 0) {
        valread = recv(client_fd, buffer, 1024, 0);
        if (valread > 0) {
            buffer[valread] = '\0';
            processClientRequest(client_fd, sessionId, playerId, buffer);
            memset(buffer, 0, sizeof(buffer));
        }
    }

    closesocket(client_fd);
    return 0;
}

void initServer(int port) {
    WSADATA wsa;
    SOCKET server_fd;
    struct sockaddr_in address;

    WSAStartup(MAKEWORD(2, 2), &wsa);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    InitializeCriticalSection(&session_mutex);

    for (int i = 0; i < MAX_SESSIONS * MAX_PLAYERS; i++) {
        client_fds[i] = 0;
    }

    while (1) {
        int addrlen = sizeof(address);
        SOCKET client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_fd != INVALID_SOCKET) {
            DWORD threadId;
            HANDLE threadHandle = CreateThread(NULL, 0, handleClient, &client_fd, 0, &threadId);
            client_threads[client_fd] = threadHandle;
        }
    }

    DeleteCriticalSection(&session_mutex);
    WSACleanup();
}


int startServer() {
    initServer(6974);
    return 0;
}