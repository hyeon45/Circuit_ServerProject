#pragma once
#include "Common.h"
#include "CarPacket.h"
#include "WorldState.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <optional>

struct ClientInfo {
	SOCKET sock;
	sockaddr_in addr;
	int playerID = -1;
	bool connected;
	std::atomic<uint8_t> button{ 0 };
};

struct CopyClientInfo {
	SOCKET sock;
	sockaddr_in addr;
	int playerID;
	bool connected;
	uint8_t button;
};

class ServerMain {
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
	
private:
	SOCKET listen_sock;
	std::vector<ClientInfo> clients;
	std::mutex worldMutex;
	mutable std::mutex clientsMutex;
	WorldState world;

};