#include "WorldState.h"
#include <algorithm>
#include <cmath>

WorldState::WorldState()
    : player(0) // Player ID
{
    Initialize();
}

// ----------------------------------------
// 게임 시작 시 초기화
// ----------------------------------------
void WorldState::Initialize()
{
    float ItemPosy = 5.0f; // 아이템 배치 높이값은 고정
    float obstaclePosy = 10.1f; // 장애물 배치 높이값 고정

    // 플레이어 초기 위치
    player.x = 100.0f;
    player.y = 10.0f;
    player.z = 1780.0f;

    // 아이템 배치
    items = {
        {0, 270.0f, ItemPosy, 1100.0f},
        {1, 300.0f, ItemPosy, 1100.0f},
        {2, 330.0f, ItemPosy, 1100.0f},
        {3, 360.0f, ItemPosy, 1100.0f},

        {4, 1000.0f, ItemPosy, 1700.0f},
        {5, 1000.0f, ItemPosy, 1730.0f},
        {6, 1000.0f, ItemPosy, 1760.0f},
        {7, 1000.0f, ItemPosy, 1790.0f},

        {8, 1840.0f, ItemPosy, 900.0f},
        {9, 1870.0f, ItemPosy, 900.0f},
        {10, 1900.0f, ItemPosy, 900.0f},
        {11, 1930.0f, ItemPosy, 900.0f},

        {12, 810.0f, ItemPosy, 600.0f},
        {13, 840.0f, ItemPosy, 600.0f},
        {14, 870.0f, ItemPosy, 600.0f},
        {15, 900.0f, ItemPosy, 600.0f}
    };

    // 장애물 배치
    obstacles = {
        {0,  60.0f,  obstaclePosy, 1500.0f, 20.0f},
        {1, 140.0f,  obstaclePosy, 1200.0f, 20.0f},
        {2,  60.0f,  obstaclePosy, 900.0f,  20.0f},
        {3, 280.0f,  obstaclePosy, 900.0f,  20.0f},
        {4, 280.0f,  obstaclePosy, 1200.0f, 20.0f},
        {5, 360.0f,  obstaclePosy, 1600.0f, 20.0f},
        {6, 500.0f,  obstaclePosy, 1700.0f, 20.0f},
        {7, 800.0f,  obstaclePosy, 1760.0f, 20.0f},
        {8, 1200.0f, obstaclePosy, 1700.0f, 20.0f},
        {9, 1500.0f, obstaclePosy, 1760.0f, 20.0f},
        {10,1920.0f, obstaclePosy, 1400.0f, 20.0f},
        {11,1920.0f, obstaclePosy, 1100.0f, 20.0f},
        {12,1860.0f, obstaclePosy, 800.0f,  20.0f},
        {13,1240.0f, obstaclePosy, 800.0f,  20.0f},
        {14,1180.0f, obstaclePosy, 1100.0f, 20.0f},
        {15,820.0f,  obstaclePosy, 900.0f,  20.0f},
        {16,900.0f,  obstaclePosy, 500.0f,  20.0f},
        {17,600.0f,  obstaclePosy, 700.0f,  20.0f},
        {18,660.0f,  obstaclePosy, 1000.0f, 20.0f},
        {19,860.0f,  obstaclePosy, 1520.0f, 20.0f}
    };

}

// ----------------------------------------
// 아이템 제거 처리 (충돌 시 호출)
// ----------------------------------------
void WorldState::RemoveItem(int itemID)
{
    
}


// ----------------------------------------
// 클라이언트 위치, 회전, 속도 데이터 동기화 반영
// ----------------------------------------
void WorldState::SyncPlayers()
{

}

// ----------------------------------------
// 물리스레드에서 입력값을 받아 가속도, 방향을 계산
// ----------------------------------------
void WorldState::ApplyInput(int playerID, uint8_t button)
{
    if (playerID < 0 || playerID >= static_cast<int>(players.size()))
        return;

    Player& p = players[playerID];

    bool forward = button & 0x01;
    bool backward = button & 0x02;
    bool turnLeft = button & 0x04;
    bool turnRight = button & 0x08;

    const float acceleration = 40.0f;
    const float rotSpeed = 15.0f * 3.1415926535f / 180.0f;

    if (forward)
        p.speed += acceleration;
    else if (backward)
        p.speed -= acceleration;
    else {
        if (p.speed > 0.0f)      p.speed = std::max(0.0f, p.speed - acceleration);
        else if (p.speed < 0.0f) p.speed = std::min(0.0f, p.speed + acceleration);
    }
    if (turnLeft)
        p.yaw -= rotSpeed * (1.0f / 60.0f);
    if (turnRight)
        p.yaw += rotSpeed * (1.0f / 60.0f);

    if (p.speed > p.maxSpeed) p.speed = p.maxSpeed;
    if (p.speed < -p.maxSpeed) p.speed = -p.maxSpeed;
}

// ----------------------------------------
// 서버에 틱 시간에 맞춰서 단순 위치를 계산
// ----------------------------------------
void WorldState::StepPhysics(float deltaTime)
{
    for (auto& p : players)
    {
        // 단순 위치 적분
        p.x += std::cos(p.yaw) * p.speed * deltaTime;
        p.z += std::sin(p.yaw) * p.speed * deltaTime;
        p.speed *= 0.98f;
    }
}

// ----------------------------------------
// 충돌한 아이템 판정
// ----------------------------------------
void WorldState::DetectItemCollisions(std::vector<CollisionInfo>& outPicked)
{
    outPicked.clear();

    const float pickupRange = 15.0f;

    for (auto& p : players) {
        for (auto& item : items) {
            float dx = p.x - item.x;
            float dz = p.z - item.z;
            float dist = dx * dx + dz * dz;

            if (dist <= pickupRange * pickupRange) {
                outPicked.push_back({ p.id,item.id });
            }
        }
    }
}

// ----------------------------------------
// 장애물 충돌 판정
// ----------------------------------------
void WorldState::ObsCollisionCheck(int playerID)
{

}