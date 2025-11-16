#include "ServerMain.h"


struct NetThreadArg {
	ServerMain* self;
	int index;
};

ServerMain::ServerMain() : 
pkt_handler(new PacketHandler(this)),
	raceStateManager(pkt_handler) {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		err_quit("WSAStartup()");
	}
}

ServerMain::~ServerMain() {
	// new 로 만든 생성자 소멸
	delete pkt_handler;

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
			std::lock_guard<std::mutex> wg(worldMutex);
			ci.playerID = static_cast<int>(world.players.size()); //일단 각 클라마다 ID 할당할 때 주는 방식으로 선정
			
			// world에 플레이어 생성
			world.players.emplace_back(ci.playerID);

			world.players.push_back(Player(ci.playerID));
			world.AddPlayer(ci.playerID);	// 플레이어 위치 설정 

		}

		int idx = -1;
		bool shouldSendStart = false;

		// clients에 등록 
		{
			std::lock_guard<std::mutex> cg(clientsMutex);
			clients.push_back(ci);
			idx = static_cast<int>((clients.size()) - 1);

			if (gameStarted.load() == false) {
				int connected = 0;
				for (const auto& c : clients)
					if (c.connected) ++connected;
				if (connected >= requiredPlayers) {
					gameStarted.store(true);
					shouldSendStart = true;
				}
			}
		}

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

		if (shouldSendStart) {
			raceStateManager.SetState(RaceState::COUNTDOWN);
		}
	}
}

#pragma pack(push,1)
struct PKT_HEADER {
	uint16_t type;
	uint16_t size;
};
#pragma pack(pop)

// -------------------------------
// 클라이언트 송수신 스레드
// -------------------------------
DWORD WINAPI ServerMain::NetThread(LPVOID arg) {
	std::unique_ptr<NetThreadArg> guard((NetThreadArg*)arg);
	ServerMain* self = guard->self;
	const int idx = guard->index;

	SOCKET sock = INVALID_SOCKET;
	int playerID = -1;

	{
		std::lock_guard<std::mutex> lg(self->clientsMutex);
		if (idx < 0 || idx >= static_cast<int>(self->clients.size()) || self->clients[idx].connected == false)
			return 0;
		sock = self->clients[idx].sock;
		playerID = self->clients[idx].playerID;
	}

	// 초기 동기화 패킷 보내주기 PKT_WorldSync 패킷 send? (일단 보류)
	// 클라가 다 접속하면 PKT_GAME_START send해주기 (원래 2인용이었는데 아직 최대 인원 못 정함) > 보류


	char headerBuf[sizeof(PKT_HEADER)];

	int retval;

	while (1) {

		// 헤더 수신
		retval = recv(sock, headerBuf, sizeof(PKT_HEADER), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}

		PKT_HEADER hdr;
		memcpy(&hdr, headerBuf, sizeof(PKT_HEADER));

		// 유효성 확인
		if (hdr.size < sizeof(PKT_HEADER))
			break;

		const int bodysize = hdr.size - sizeof(PKT_HEADER);
		std::vector<char> body(bodysize);
		if (bodysize > 0) {
			retval = recv(sock, body.data(), bodysize, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
		}

		// 패킷 처리
		switch (hdr.type) 
		{
		case PKT_CAR_MOVE:
		{	// 클라 입력 패킷 반영
			if ((int)body.size() < (int)sizeof(PKT_CarMove)) {
				break;
			}
			const PKT_CarMove* pkt = reinterpret_cast<const PKT_CarMove*>(body.data());
			uint8_t btn = pkt->button;
			{
				std::lock_guard<std::mutex> lg(self->clientsMutex);
				if (idx >= 0 && idx < (int)self->clients.size() && self->clients[idx].connected) {
					self->clients[idx].button = btn;
				}
			}
			break;
		}
		default:
			break;
		}
		// 따로 send 하고 싶은 패킷이 있으면 추가 send는 패킷 핸들러가 직접 send 해도 된다.
		// physicsThread에서 틱마다 PKT_WorldSync send 해줌
	}

	// 연결 끊깃 것 정리
	{
		std::lock_guard<std::mutex> lg(self->clientsMutex);
		if (idx >= 0 && idx < static_cast<int>(self->clients.size())) {
			self->clients[idx].connected = false;
			if (self->clients[idx].sock != INVALID_SOCKET) {
				closesocket(self->clients[idx].sock);
				self->clients[idx].sock = INVALID_SOCKET;
			}
		}
	}
	printf("[NetThread] player %d disconnected\n", playerID);
	return 0;
}

struct InputSnapshot {
	int      playerID;
	uint8_t  button;
};

// -------------------------------
// 물리 스레드
// -------------------------------
DWORD WINAPI ServerMain::PhysicsThread(LPVOID arg) {
	ServerMain* self = (ServerMain*)arg;

	using clock = std::chrono::steady_clock;
	auto next = clock::now();
	const auto tick = std::chrono::milliseconds(16); // 60fps
	const float deltaTime = 1.0f / 60.0f;

	while (1) {
		next += tick;

		// -------------------------
		// 1. 카운트다운 진행
		// -------------------------
		self->raceStateManager.StartCountdown(deltaTime);

		// COUNTDOWN 중에는 물리 처리/입력 무시하는게 자연스러움
		if (self->raceStateManager.GetState() == RaceState::COUNTDOWN)
		{
			std::this_thread::sleep_until(next);
			continue; // 물리 계산 X
		}

		std::vector<InputSnapshot> inputs;
		{
			std::lock_guard<std::mutex> cg(self->clientsMutex);
			inputs.reserve(self->clients.size());
			for (const auto& c : self->clients) {
				if (!c.connected) continue;
				inputs.push_back(InputSnapshot{ c.playerID, c.button });
			}
		}

		std::vector<CollisionInfo> picked;
		int winner = -1; // CheckFinsh() 만들면 추가
		{
			std::lock_guard<std::mutex> lg(self->worldMutex);

			// 입력 반영
			for (const auto& in : inputs) {
				const int pid = in.playerID;
				if (pid >= 0 && pid < (int)self->world.players.size()) {
					self->world.ApplyInput(pid, in.button);
				}
			}

			// 물리 적분,스텝
			self->world.StepPhysics(1.0f / 60.0f);

			// 충돌 아이템 판정
			
			self->world.DetectItemCollisions(picked);

			// 아이템 지속 효과 적용 및 지속시간 갱신
			for (auto& player : self->world.players) {
				player.UpdateEffect(deltaTime);  // 지속시간 관리 (3초, 5초 등)
			}

			// winner - self->world.CheckFinish(); // 아직 미구현
			
		}
		for (const auto& e : picked) {
			ItemType itemType = ItemType::None;
			{
				std::lock_guard<std::mutex> lg(self->worldMutex);
				ItemType itemType = self->world.GetItemType(e.itemID);	//삭제 전 조회
			}

			self->ProcessItemDelete(e.playerID, e.itemID);

			if (itemType != ItemType::None &&e.playerID >= 0 && e.playerID < (int)self->world.players.size()) {
				self->world.players[e.playerID].ApplyItemEffect(itemType);
			}
		}


		if (winner >= 0) {
			self->BroadcastFinalResult();
		}

		self->pkt_handler->SendWorldState();

		// 틱 시간 보정. 다음 프레임 목표까지 쉼.
		std::this_thread::sleep_until(next);
		
	}
	return 0;
	
}

// -------------------------------
// 아이템 삭제 처리
// -------------------------------
void ServerMain::ProcessItemDelete(int playerID, int itemID) {
	Item removed;

	{
		std::lock_guard<std::mutex> lg(worldMutex);
		// 아이템 삭제만 수행
		if (!world.RemoveItem(itemID, removed))
			return;

	}

	// 삭제 전 패킷에 미리 저장
	PKT_ItemDelete pkt;
	pkt.type = PKT_ITEM_DELETE;
	pkt.itemID = itemID;
	pkt.posx = removed.x;
	pkt.posy = removed.y;
	pkt.posz = removed.z;

	// 클라이언트에게 삭제 패킷 전송
	pkt_handler->ItemDelete(pkt);
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
	return CopyClientInfo{ c.sock,c.addr,c.playerID,c.connected,c.button };
}

// -------------------------------
// 클라 스냅샷 가져오기
// -------------------------------
std::vector<CopyClientInfo> ServerMain::GetClientsnapshot() const {
	std::lock_guard<std::mutex> cl(clientsMutex);
	std::vector<CopyClientInfo> snapshot;
	snapshot.reserve(clients.size());
	for (const auto& c : clients) {
		if (c.connected && c.sock != INVALID_SOCKET) {
			snapshot.push_back(CopyClientInfo{
				c.sock,
				c.addr,
				c.playerID,
				c.connected,
				c.button
				});
		}
	}
	return snapshot;
}

// -------------------------------
// 플레이어들 현재 상태 패킷정보 가져오기
// -------------------------------
std::vector<PKT_WorldSync> ServerMain::WorldSyncPackets() const {
	std::lock_guard<std::mutex> lg(worldMutex);
	
	const auto& players = world.GetPlayers();

	std::vector<PKT_WorldSync> pkts;
	pkts.reserve(players.size());

	for (const auto& p : players) {
		PKT_WorldSync pkt;
		pkt.type = PKT_WORLD_SYNC;
		pkt.playerID = p.id;
		pkt.posx = p.x;
		pkt.posy = p.y;
		pkt.posz = p.z;
		pkt.yaw = p.yaw;
		pkt.scale = p.scale;
		pkt.shield = p.shield ? 1u : 0u;
		pkts.push_back(pkt);
	}
	return pkts;
}