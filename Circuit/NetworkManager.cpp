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

//---------------------------------
// Send 패킷 보내기
//---------------------------------
bool NetworkManager::SendPacket(const void* pkt, int size)
{
    if (sock == INVALID_SOCKET)
        return false;

    int retval = send(sock, (const char*)pkt, size, 0);

    if (retval == SOCKET_ERROR) {
        err_quit("send_fail()");
        return false;
    }

    // 보낸 크기와 패킷 크기가 다르면 문제
    if (retval != size) {
        err_quit("pkt_size()");
        return false;
    }
        
    return true;
}

//---------------------------------
// PKT_CarMove 패킷 생성 및 보내기
//---------------------------------
bool NetworkManager::SendCarMove(int playerID, uint8_t button)
{
    PKT_CarMove pkt{};
    pkt.type = PKT_CAR_MOVE;
    pkt.playerID = playerID;
    pkt.button = button;

    return SendPacket(&pkt, sizeof(pkt));
}