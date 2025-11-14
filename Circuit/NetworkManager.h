#pragma once
#include "Common.h"  

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    bool Connect();   // SERVERIP, SERVERPORT 전역 변수 사용
    void Disconnect();

    SOCKET GetSocket() const { return sock; }   // 소켓 호출

private:
    SOCKET sock; // 클라 소켓 저장 
};