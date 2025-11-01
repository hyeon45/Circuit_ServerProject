#pragma once
#include <vector>
#include "Player.h"
#include "Item.h"

class WorldState
{
public:
    Player player;           // 현재 1인용 버전
    std::vector<Item> items; // 맵 내 존재하는 아이템들

    WorldState();

    void Initialize();           // 초기 세팅
    void RemoveItem(int itemID); // 아이템 삭제 처리
    void SyncPlayers();          // 클라이언트를 받아 위치, 속도, 데이터를 동기화해 구조체를 반영
};
