#pragma once
#include "Common.h"  
#include "CarPacketClient.h"


class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    bool Connect();   // SERVERIP, SERVERPORT 전역 변수 사용
    void Disconnect();

    
    bool SendPacket(const void* pkt, int size); // 패킷 전송 공용 함수
    bool SendCarMove(int playerID, uint8_t button); // 패킷 전송 공용 함수

    SOCKET GetSocket() const { return sock; }   // 소켓 호출

private:
    SOCKET sock; // 클라 소켓 저장 

    // Recv 전용 스레드 관련
    HANDLE hRecvThread = nullptr;
    std::atomic<bool> running{ false };

    static DWORD WINAPI RecvThread(LPVOID arg);  // 스레드 함수

};