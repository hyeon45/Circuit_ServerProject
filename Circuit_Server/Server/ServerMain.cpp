#include "ServerMain.h"
#include "PacketHandler.h"

ServerMain::ServerMain() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("WSAStartup failed\n");
		exit(1);
	}
}

ServerMain::~ServerMain() {
	for (auto& c : clients) {
		closesocket(c.sock);
	}
	closesocket(listen_sock);
	WSACleanup();
}

// -------------------------------
// 서버 초기화
// -------------------------------
bool ServerMain::InitServer(int port) {

}

// -------------------------------
// 클라이언트 접속 대기
// -------------------------------
void ServerMain::AcceptClient() {

}

// -------------------------------
// 클라이언트 송수신 스레드
// -------------------------------
DWORD WINAPI ServerMain::NetThread(LPVOID arg) {

}

// -------------------------------
// 물리 스레드
// -------------------------------
DWORD WINAPI ServerMain::PhysicsThread(LPVOID arg) {

}

// -------------------------------
// 아이템 삭제 처리
// -------------------------------
void ServerMain::ProcessItemDelete(int playerID, int itemID) {

}

// -------------------------------
// 최종 결과 전송
// -------------------------------
void ServerMain::BroadcastFinalResult() {

}

