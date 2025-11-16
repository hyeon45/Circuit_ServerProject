#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Game.h"
#include "NetworkManager.h" 

NetworkManager g_Network;

// -----------------------------------
// 프로그램 시작점
// -----------------------------------
int main(int argc, char** argv)
{
    if (!g_Network.Connect()) {
        printf("서버 연결 실패...\n");
        return 0;   // 연결 안 되면 게임 실행 안 함
    }

    // Game 객체 생성 및 실행
    Game game;
    game.Run(argc, argv);

    return 0;
}
