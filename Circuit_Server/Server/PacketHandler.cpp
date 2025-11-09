#include "PacketHandler.h"
#include "ServerMain.h"
#include "CarPacket.h"


// -------------------------------------------------------------------------
// 클라 입력 패킷을 서버 내부 상태에 반영
// -------------------------------------------------------------------------
void PacketHandler::ProcessCarMove(int playerID, const PKT_CarMove& pkt) {
    if (!server_) return;

    // GetClient() 사용 (복사본 스냅샷 얻기)
    auto optClient = server_->GetClient(playerID);
    if (!optClient.has_value())   // 클라이언트 존재 여부 확인
        return;

    const auto& client = *optClient;  // optional에서 실제 값 꺼내기
    if (!client.connected)
        return;

    // 여기서 client.button, client.playerID, client.sock 등 접근 가능
    // 단, 이건 복사본(ClientSnapshot)이므로 직접 수정은 안 됨
    uint8_t buttonState = pkt.button;

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

    const auto clients = server_->GetClientsnapshot();
    const auto pkts = server_->WorldSyncPackets();

    for (const auto& cl : clients) {
        for (const auto& pkt : pkts) {
            send(cl.sock, reinterpret_cast<const char*>(&pkt), sizeof(pkt), 0);
        }
    }
}

// -------------------------------------------------------------------------
// PKT_GAME_RESULT 패킷을 전송 <최종 승자 정보 전송>
// -------------------------------------------------------------------------
void PacketHandler::SendGameResult(int winnerID) {

}