#pragma once

class Player
{
public:
    int id;           // 플레이어 ID
    float x, y, z;    // 위치
    float yaw;        // 회전(방향)
    float speed;      // 현재 속도
    float maxSpeed;   // 최대 속도
    float acceleration; // 가속도
    float scale;      // 크기(아이템 효과용)
    bool shield;      // 보호막 상태
    float speedBoostTimer; // 속도 아이템 지속시간
    float shrinkTimer;     // 축소 아이템 지속시간

    Player(int pid = 0);
};
