#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Game.h"

// -----------------------------------
// 프로그램 시작점
// -----------------------------------
int main(int argc, char** argv)
{
    // Game 객체 생성 및 실행
    Game game;
    game.Run(argc, argv);

    return 0;
}
