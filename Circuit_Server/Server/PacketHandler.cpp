#include "PacketHandler.h"
#include "ServerMain.h"
#include "CarPacket.h"


// -------------------------------------------------------------------------
// 클라 입력 패킷을 서버 내부 상태에 반영
// -------------------------------------------------------------------------
void PacketHandler::ProcessCarMove(int playerID, const PKT_CarMove& pkt) {
    if (!server_) return;

    if (playerID < 0 || playerID >= static_cast<int>(server_->clients.size()))
        return;

    ClientInfo& client = server_->clients[playerID];
    if (!client.connected)
        return;

    ClientInfo& client = server_->clients[playerID];
    if (!client.connected)
        return;
}

// -------------------------------------------------------------------------
// PKT_ITEM_DELETE 패킷을 <전송 실제 삭제는 WorldState::RemoveItem에서 수행>
// -------------------------------------------------------------------------
void PacketHandler::ItemDelete(int itemID) {
	
}

// -------------------------------------------------------------------------
// PKT_WORLD_SYNC 패킷을 전송 <플레이어별 상태를 모든 클라에 전송>
// -------------------------------------------------------------------------
void PacketHandler::SendWorldState() {

}

// -------------------------------------------------------------------------
// PKT_GAME_RESULT 패킷을 전송 <최종 승자 정보 전송>
// -------------------------------------------------------------------------
void PacketHandler::SendGameResult(int winnerID) {

}