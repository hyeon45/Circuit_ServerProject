#pragma once
#include "Common.h"
#include "CarPacket.h"
//#include "ServerMain.h"

class ServerMain;

class PacketHandler {
public:
	PacketHandler(ServerMain* server) : server_(server) {};
	void ProcessCarMove(int playerID, const PKT_CarMove& pkt);
	void ItemDelete(const PKT_ItemDelete& pkt);
	void SendWorldState();
	void SendGameResult(int winnerID);
	
private:
	ServerMain* server_;	//PacketHandler에서 ServerMain안에 있는 clients 등의 값을 받아오기 위해 사용
};