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
// ���� �ʱ�ȭ
// -------------------------------
bool ServerMain::InitServer(int port) {

}

// -------------------------------
// Ŭ���̾�Ʈ ���� ���
// -------------------------------
void ServerMain::AcceptClient() {

}

// -------------------------------
// Ŭ���̾�Ʈ �ۼ��� ������
// -------------------------------
DWORD WINAPI ServerMain::NetThread(LPVOID arg) {

}

// -------------------------------
// ���� ������
// -------------------------------
DWORD WINAPI ServerMain::PhysicsThread(LPVOID arg) {

}

// -------------------------------
// ������ ���� ó��
// -------------------------------
void ServerMain::ProcessItemDelete(int playerID, int itemID) {

}

// -------------------------------
// ���� ��� ����
// -------------------------------
void ServerMain::BroadcastFinalResult() {

}

