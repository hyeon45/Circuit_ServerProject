#pragma once
#include "Common.h"
#include "WorldState.h"
#include "PacketHandler.h"
#include "CarPacket.h" 
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <optional>
#include "RaceStateManager.h"

struct ClientInfo {
	SOCKET sock;
	sockaddr_in addr;
	int playerID = -1;
	bool connected;
	uint8_t button{ 0 };
};

struct CopyClientInfo {
	SOCKET sock;
	sockaddr_in addr;
	int playerID;
	bool connected;
	uint8_t button;
};

class ServerMain {
	friend class PacketHandler;
public:
	ServerMain();
	~ServerMain();

	bool InitServer(int port);
	void AcceptClient();
	static DWORD WINAPI NetThread(LPVOID arg);
	static DWORD WINAPI PhysicsThread(LPVOID arg);
	void ProcessItemDelete(int playerID, int itemID);
	void BroadcastFinalResult();

	// 읽기용 참조 반환 
	const std::vector<ClientInfo>& GetClients() const { return clients; }

	// 단일 클라 접근용
	std::optional<CopyClientInfo> GetClient(int id) const;

	// 클라 스냅샷 가져오기
	std::vector<CopyClientInfo> GetClientsnapshot() const;
	// 플레이어들 현재 상태 패킷정보 가져오기
	std::vector<PKT_WorldSync> WorldSyncPackets() const;

private:
	SOCKET listen_sock;
	std::vector<ClientInfo> clients;
	mutable std::mutex worldMutex;
	mutable std::mutex clientsMutex;
	WorldState world;
	PacketHandler* pkt_handler;
	RaceStateManager raceStateManager;
	std::atomic<bool> gameStarted{ false };
	int requiredPlayers = 2;
};