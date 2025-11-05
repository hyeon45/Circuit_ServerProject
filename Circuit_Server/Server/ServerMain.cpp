#include "ServerMain.h"


struct NetThreadArg {
	ServerMain* self;
	int index;
};

ServerMain::ServerMain() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		err_quit("WSAStartup()");
	}

	// PacketHandler 가 ServerMain 을 참조할 수 있도록 생성자 추가
	pkt_handler = new PacketHandler(this);

	// ServerMain 의 월드 공유
	itemEffectManager = new ItemEffectManager(world);
}

ServerMain::~ServerMain() {
	// new 로 만든 생성자 소멸
	delete pkt_handler;
	delete itemEffectManager;

	// listen 소켓 정리
	if (listen_sock != INVALID_SOCKET) {
		closesocket(listen_sock);
		listen_sock = INVALID_SOCKET;
	}

	// 클라 소켓 정리
	for (auto& c : clients) {
		if (c.sock != INVALID_SOCKET) {
			closesocket(c.sock);
			c.sock = INVALID_SOCKET;
		}
	}

	WSACleanup();
}

// -------------------------------
// 서버 초기화
// -------------------------------
bool ServerMain::InitServer(int port) {
	
	int retval;

	// 소켓 생성
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) {
		err_quit("socket()");
		return false;
	}

	// bind
	sockaddr_in serveraddr{};
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(static_cast<u_short>(port));
	retval = bind(listen_sock, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit("bind()");
		return false;
	}

	// listen
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		err_quit("listen()");
		return false;
	}

	return true;
}

// -------------------------------
// 클라이언트 접속 대기
// -------------------------------
void ServerMain::AcceptClient() {

	while (1) {

		sockaddr_in clientaddr{};
		int addrlen = sizeof(clientaddr);
		HANDLE hThread;

		// accept
		SOCKET client_sock = accept(listen_sock, reinterpret_cast<sockaddr*>(&clientaddr), &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			continue;
		}

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN]{};
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		int cport = ntohs(clientaddr.sin_port);
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", addr, cport);

		// clients 에 데이터 등록
		ClientInfo ci{};
		ci.sock = client_sock;
		ci.addr = clientaddr;
		ci.connected = true;

		{
			std::lock_guard<std::mutex> lg(worldMutex);
			ci.playerID = static_cast<int>(world.players.size()); //일단 각 클라마다 ID 할당할 때 주는 방식으로 선정
			clients.push_back(ci);
			// world에 플레이어 생성
			world.players.push_back(Player(ci.playerID));
		}

		int idx = static_cast<int>((clients.size()) - 1);

		// 클라이언트 별 스레드 생성
		NetThreadArg* arg = new NetThreadArg{ this,idx };
		hThread = CreateThread(NULL, 0, &ServerMain::NetThread, arg, 0, NULL);
		if (hThread == NULL) {
			err_display("CreateThread(NetThread)");
			closesocket(client_sock);
			// 필요하다면 std::lock_guard<std::mutex> lg(worldMutex); 으로보호
			clients[idx].connected = false;
			clients[idx].sock = INVALID_SOCKET;
			delete arg;
			continue;
		}
		CloseHandle(hThread);

	}
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
	// 서버에서 아이템 제거
	world.RemoveItem(itemID);

	// 클라이언트에게 삭제 패킷 전송
	pkt_handler->ItemDelete(itemID);

	// 아이템 효과 발동 (아직 미구현)
	itemEffectManager->ApplyItemEffect(playerID, itemID);
}

// -------------------------------
// 최종 결과 전송
// -------------------------------
void ServerMain::BroadcastFinalResult() {

}

// -------------------------------
// 단일 클라 접근용
// -------------------------------
std::optional<CopyClientInfo> ServerMain::GetClient(int id) const {
	std::lock_guard<std::mutex> lg(clientsMutex);
	if (id < 0 || id >= static_cast<int>(clients.size())) return std::nullopt;
	const auto& c = clients[id];
	return CopyClientInfo{ c.sock,c.addr,c.playerID,c.connected,c.button.load() };
}
