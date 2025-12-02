#include "WorldState.h"
#include <algorithm>
#include <cmath>
#include <random>


WorldState::WorldState()
{
    Initialize();
}


// ----------------------------------------
// 게임 시작 시 플레이어 위치값
// ----------------------------------------
void WorldState::AddPlayer(int playerID)
{
    // 이미 존재하는 ID는 추가하지 않음
    for (auto& p : players)
        if (p.id == playerID)
            return;

    // 새 플레이어 생성
    Player p(playerID);

    // ID별 초기 좌표 설정
    if (playerID == 0) {
        // 디버그용
        // p.x = 1000.0f; p.y = 10.0f; p.z = 1500.0f;
        p.x = 80.0f; p.y = 10.0f; p.z = 1780.0f;
    }
    else if (playerID == 1) {
        p.x = 130.0f; p.y = 10.0f; p.z = 1780.0f;
    }

    players.push_back(p);
}

// ----------------------------------------
// 플레이어 Disconneted 가 되었을 때, 플레이어값 지우기.
// ----------------------------------------
void WorldState::DeletePlayer(int playerID) {
    players.erase(
        std::remove_if(players.begin(), players.end(),
            [&](const Player& p) { return p.id == playerID; }),
        players.end()
    );
}

// ----------------------------------------
// 게임 시작 시 초기화
// ----------------------------------------
void WorldState::Initialize()
{
    float ItemPosy = 5.0f; // 아이템 배치 높이값은 고정
    float obstaclePosy = 10.1f; // 장애물 배치 높이값 고정

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

    // 맵 충돌박스 생성
    roadBounds = {
        {40.0f, 160.0f, 490.0f, 1800.0f},
        {40.0f, 390.0f, 490.0f, 620.0f},
        {250.0f, 390.0f, 490.0f, 1800.0f},
        {250.0f, 1940.0f, 1680.0f, 1800.0f},
        {1820.0f, 1940.0f, 300.0f, 1800.0f},
        {1150.0f, 1940.0f, 300.0f, 420.0f},
        {1150.0f, 1250.0f, 300.0f, 1260.0f},
        {800.0f, 1250.0f, 1170.0f, 1260.0f},
        {800.0f, 920.0f, 70.0f, 1260.0f},
        {590.0f, 920.0f, 70.0f, 200.0f},
        {590.0f, 710.0f, 70.0f, 1540.0f},
        {590.0f, 1140.0f, 1440.0f, 1540.0f}
    };

    // ----------------------------------------
    // 아이템 타입 랜덤 지정
    // ----------------------------------------
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, 3); // 1~3 (Grow, Shrink, SpeedBoost)

    for (auto& item : items) {
        item.type = static_cast<ItemType>(dist(rd));
    }

    // 결승선 범위 지정
    finishTrigger = FinishLineTrigger(
        1093.f, 1140.f,   // X 범위
        1440.f, 1540.f    // Z 범위
    );
}

// ----------------------------------------
// 아이템 제거 처리 (충돌 시 호출)
// ----------------------------------------
bool WorldState::RemoveItem(int itemID, Item& removedOut)
{
    auto it = std::find_if(items.begin(), items.end(),
        [itemID](const Item& a) { return a.id == itemID; });

    if (it == items.end())
        return false;
    removedOut = *it;
    items.erase(it);
    return true;

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
        p.x += std::sin(p.yaw) * p.speed * deltaTime;
        p.z += -std::cos(p.yaw)* p.speed* deltaTime;
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
    Player& p = players[playerID];

    // 플레이어 충돌 반경 계산
    float basePlayerRadius = 20.0f;               // 기본 충돌 반경
    float playerRadius = basePlayerRadius * p.scale;

    for (auto& ob : obstacles)
    {
        float dx = p.x - ob.x;
        float dz = p.z - ob.z;

        float distSq = dx * dx + dz * dz;

        float collideDist = playerRadius + ob.radius;

        if (distSq < collideDist * collideDist)
        {
            // 1회 충돌 무시
            if (p.shield)
            {
                p.scale = 1.0f;
                p.shield = false;
                continue;
            }

            float dist = sqrt(distSq);
            if (dist < 0.0001f) dist = 0.0001f;

            // 정규화 방향 벡터(장애물 → 플레이어)
            float nx = dx / dist;
            float nz = dz / dist;

            // 겹친 만큼 튕겨내기
            float push = (collideDist - dist);
            p.x += nx * push;
            p.z += nz * push;

            // 속도 중단
            p.speed = 0.0f;
        }
    }
}

// ----------------------------------------
// 아이템 타입 반환 함수
// ----------------------------------------
ItemType WorldState::GetItemType(int itemID)
{
    for (const auto& item : items) {
        if (item.id == itemID)
            return item.type;
    }
    return ItemType::None;
}

// ----------------------------------------
// 도로 밖 주행 여부 체크
// ----------------------------------------
bool WorldState::IsInsideRoad(float x, float z)
{
    for (auto& b : roadBounds)
    {
        if (x >= b.minX && x <= b.maxX &&
            z >= b.minZ && z <= b.maxZ)
            return true;
    }
    return false;
}

// ----------------------------------------
// 도로 밖 주행 충돌 여부
// ---------------------------------------- 
void WorldState::RoadBoundaryCollision(int playerID)
{
    Player& p = players[playerID];

    float x = p.x;
    float z = p.z;

    // 도로 안에 있으면 충돌 없음
    if (IsInsideRoad(x, z)) return;

    float bestDist = FLT_MAX;
    float bestX = x;
    float bestZ = z;

    // 모든 도로 바운더리를 검사
    for (auto& b : roadBounds)
    {
        float clampedX = std::clamp(x, b.minX, b.maxX);
        float clampedZ = std::clamp(z, b.minZ, b.maxZ);

        float dx = x - clampedX;
        float dz = z - clampedZ;
        float distSq = dx * dx + dz * dz;

        if (distSq < bestDist)
        {
            bestDist = distSq;
            bestX = clampedX;
            bestZ = clampedZ;
        }
    }

    // 가장 가까운 “도로 안쪽 위치”로 밀기
    p.x = bestX;
    p.z = bestZ;

    // 속도 0으로 멈춤
    p.speed = 0.0f;
}

// ----------------------------------------
// 월드 리셋
// ---------------------------------------- 
void WorldState::Reset()
{
    players.clear();
    items.clear();

    Initialize();
}