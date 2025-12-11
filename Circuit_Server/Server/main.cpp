#include "ServerMain.h"
#include "Common.h"

int main()
{
    ServerMain server;

    if (!server.InitServer(9000)) {
        printf("InitServer() 실패\n");
        return 1;
    }

    // 물리 갱신용 스레드 (ServerMain::PhysicsThread가 static이면 이렇게)
    HANDLE hPhysics = CreateThread(
        nullptr, 0,
        ServerMain::PhysicsThread,
        &server,          // LPVOID arg 로 this 포인터 전달
        0, nullptr
    );

    printf("[TCP 서버] 초기화 완료. 클라이언트 접속 대기 중...\n");
    server.AcceptClient();   // 여기서 accept() + NetThread 생성

    // 나중에 종료 로직 추가하고 싶으면 WaitForSingleObject 등으로 정리
    CloseHandle(hPhysics);
    return 0;
}