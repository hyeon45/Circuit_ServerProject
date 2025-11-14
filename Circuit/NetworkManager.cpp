#include "NetworkManager.h"

char* SERVERIP = (char*)"127.0.0.1";
int SERVERPORT = 9000;

//---------------------------------
// 소켓 초기화
//---------------------------------
NetworkManager::NetworkManager() : sock(INVALID_SOCKET) {

}

//---------------------------------
// closeSocket, WSACleanup 관리
//---------------------------------
NetworkManager::~NetworkManager() {
	Disconnect();
	WSACleanup();
}

//---------------------------------
// 서버 연결 
//---------------------------------
bool NetworkManager::Connect() {

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return false;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        return false;

    sockaddr_in serveraddr{};
    serveraddr.sin_family = AF_INET;

    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);

    int ret = connect(sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
    if (ret == SOCKET_ERROR)
        return false;

    return true;
}

//---------------------------------
// 소켓 닫기
//---------------------------------
void NetworkManager::Disconnect() {

	if (sock != INVALID_SOCKET) {
		closesocket(sock);
		sock = INVALID_SOCKET;
	}
}