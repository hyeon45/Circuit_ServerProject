#include "WorldState.h"

WorldState::WorldState()
    : player(0) // Player ID
{
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