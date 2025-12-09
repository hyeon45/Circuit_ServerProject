    #include "NetworkManager.h"
    #include "Game.h"
    #include "Item.h"

    // 테스팅용
    //char* SERVERIP = (char*)"10.59.44.2";
    char* SERVERIP = (char*)"127.0.0.1";
    int SERVERPORT = 9000;

    //---------------------------------
    // 소켓 초기화
    //---------------------------------
    NetworkManager::NetworkManager() : sock(INVALID_SOCKET) {

    }

    //---------------------------------
    // closeSocket, WSACleanup 관리
    //---------------------------------
    NetworkManager::~NetworkManager() {
	    Disconnect();
    }

    //---------------------------------
    // 서버 연결 
    //---------------------------------
    bool NetworkManager::Connect() {

        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            return false;

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET)
            return false;

        sockaddr_in serveraddr{};
        serveraddr.sin_family = AF_INET;

        inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
        serveraddr.sin_port = htons(SERVERPORT);

        int retval = connect(sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
        if (retval == SOCKET_ERROR)
            return false;

        return true;
    }

    //---------------------------------
    // 소켓 닫기
    //---------------------------------
    void NetworkManager::Disconnect() {

        // 스레드 정지 요청
        running = false;

        if (sock != INVALID_SOCKET) {
            closesocket(sock);
            sock = INVALID_SOCKET;
        }

        if (hRecvThread) {
            WaitForSingleObject(hRecvThread, INFINITE);
            CloseHandle(hRecvThread);
            hRecvThread = nullptr;
        }

        WSACleanup();
    }

    //---------------------------------
    // Send 패킷 보내기
    //---------------------------------
    bool NetworkManager::SendPacket(const void* pkt, int size)
    {
        if (sock == INVALID_SOCKET)
            return false;

        int retval = send(sock, (const char*)pkt, size, 0);

        if (retval == SOCKET_ERROR) {
            err_quit("send_fail()");
            return false;
        }

        // 보낸 크기와 패킷 크기가 다르면 문제
        if (retval != size) {
            err_quit("pkt_size()");
            return false;
        }
        
        return true;
    }

    //---------------------------------
    // PKT_CarMove 패킷 생성 및 보내기
    //---------------------------------
    bool NetworkManager::SendCarMove(int playerID, uint8_t button)
    {
       /* PKT_CarMove pkt{};
        pkt.type = PKT_CAR_MOVE;
        pkt.playerID = playerID;
        pkt.button = button;
        return SendPacket(&pkt, sizeof(pkt));*/

        PKT_HEADER hdr{};
        hdr.type = PKT_CAR_MOVE;
        hdr.size = sizeof(PKT_HEADER) + sizeof(PKT_CarMove); // ★ 1 아니고 struct 전체

        PKT_CarMove body{};
        body.playerID = playerID;   // 서버에서 안 쓰더라도 struct 맞춰서 채워주기
        body.button = button;

        char buf[sizeof(PKT_HEADER) + sizeof(PKT_CarMove)];
        memcpy(buf, &hdr, sizeof(hdr));
        memcpy(buf + sizeof(hdr), &body, sizeof(body));

        return SendPacket(buf, sizeof(buf));
    }

    //---------------------------------
    // RecvThread 생성함수
    //---------------------------------
    bool NetworkManager::StartRecvThread() {
        if (running) return true;

        running = true;

        hRecvThread = CreateThread(nullptr, 0, RecvThread, this, 0, nullptr);
        if (!hRecvThread) {
            running = false;
            return false;
        }
        return true;
    }


    //---------------------------------
    // RecvThread 함수
    //---------------------------------
    DWORD WINAPI NetworkManager::RecvThread(LPVOID arg) 
    {
        NetworkManager* self = static_cast<NetworkManager*>(arg);

        while (self->running) {
            uint16_t header;
            //PKT_WorldSync pkt{};
            int retval = recv(self->sock, (char*)&header, sizeof(header), MSG_WAITALL);

            if (retval == 0) {
                self->running = false;
                break;
            }

            if (retval == SOCKET_ERROR) {
                int err = WSAGetLastError();
                if (!self->running)
                    break;
                err_display("recv()");
                self->running = false;
                break;
            }

            //if (retval != sizeof(pkt)) { // 나중에 추가
            //    continue;
            //}

            //if (pkt.type != PKT_WORLD_SYNC) { // 얘도 나중에 늘어나면 swtich로 추가
            //    continue;
            //}
            switch (header) {
            case PKT_GAME_START: {
                 PKT_GameStart pkt;
                pkt.type = header;
                recv(self->sock,
                    ((char*)&pkt) + sizeof(header),
                    sizeof(pkt) - sizeof(header),
                    MSG_WAITALL);

                std::cout << "playerID = " << pkt.playerID << std::endl;
                if (Game::GetInstance()) {
                    Game::GetInstance()->SetPlayerID(pkt.playerID);
                }
                break;
            }
            case PKT_WORLD_SYNC: {
                PKT_WorldSync pkt;
                pkt.type = header;

                recv(self->sock,
                    ((char*)&pkt) + sizeof(header),
                    sizeof(pkt) - sizeof(header),
                    MSG_WAITALL);
                //std::cout << "recv!!!!" << std::endl;
                if (Game::GetInstance()) {
                    Game::GetInstance()->OnWorldSync(pkt);
                }
                break;
            }
            case PKT_ITEM_DELETE: {
                PKT_ItemDelete pkt;
                pkt.type = header;
                recv(self->sock,
                    ((char*)&pkt) + sizeof(header),
                    sizeof(pkt) - sizeof(header),
                    MSG_WAITALL);
                self->ProcessItemDelete(pkt);
                break;
            }
            case PKT_GAME_RESULT: {
                PKT_GameResult pkt;
                pkt.type = header;

                recv(self->sock,
                    ((char*)&pkt) + sizeof(header),
                    sizeof(pkt) - sizeof(header),
                    MSG_WAITALL);
                if (Game::GetInstance()) {
                    Game::GetInstance()->ShowResult(pkt.winnerID);
                }
                break;
            }
            }
        }

        return 0;
    }

    //---------------------------------
    // 아이템 패킷 받아서 ApplyItemDelete 로 보내주기
    //---------------------------------
    void NetworkManager::ProcessItemDelete(const PKT_ItemDelete& pkt) 
    {
        if (!itemManager) return;
        itemManager->ApplyItemDelete(pkt.itemID);
    }