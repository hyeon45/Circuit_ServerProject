#pragma once
#include "Common.h"
#include "CarPacket.h"
#include "WorldState.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

struct ClientInfo {
	SOCKET sock;
	sockaddr_in addr;
	int playerID = -1;
	bool connected;
	std::atomic<uint8_t> button{ 0 };
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
private:
	SOCKET listen_sock;
	std::vector<ClientInfo> clients;
	std::mutex worldMutex;
	WorldState world;

};