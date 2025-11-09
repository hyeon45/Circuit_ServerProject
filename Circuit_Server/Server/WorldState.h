#pragma once
#include <vector>
#include "Player.h"
#include "Item.h"
#include "Obstacle.h"

// 충돌 체크 할때 사용할 구조체
struct CollisionInfo {
    int playerID;
    int itemID;
};

class WorldState
{
public:
    Player player;           // 현재 1인용 버전
    std::vector<Player> players;
    std::vector<Item> items; // 맵 내 존재하는 아이템들
    std::vector<Obstacle> obstacles;    // 맵에 존재하는 장애물들과 충돌판정 수치값

    // 읽기용 참조 반환
    const std::vector<Player>& GetPlayers() const { return players; } 

    WorldState();

    void Initialize();           // 초기 세팅
    void RemoveItem(int itemID); // 아이템 삭제 처리
    void SyncPlayers();          // 클라이언트를 받아 위치, 속도, 데이터를 동기화해 구조체를 반영
    void ApplyInput(int playerID, uint8_t button);
    void StepPhysics(float deltaTime);
    void DetectItemCollisions(std::vector<CollisionInfo>& outPicked);
    void ObsCollisionCheck(int playerID); // 장애물 충돌 판정 인자는 플레이어 ID로 해놨음 
    ItemType GetItemType(int itemID);  // 아이템 타입 반환
};
