#include <winsock2.h>

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

void initGameSession(GameSession *session);
int createGameSession();
int joinGameSession(int sessionId, SOCKET client_fd, const char *username);
void sendGameStateToClients(int sessionId);
void processClientRequest(SOCKET client_fd, int sessionId, int playerId, char *request);
DWORD WINAPI handleClient(LPVOID arg);
DWORD WINAPI serverThread(int port);
int startGameServer();
