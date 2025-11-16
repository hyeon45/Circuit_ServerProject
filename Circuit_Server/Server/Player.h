#pragma once
#include <iostream>
#include "Item.h"

class Player
{
public:
    int id;                 // 플레이어 ID
    float x, y, z;          // 위치
    float yaw;              // 회전(방향)
    float speed;            // 현재 속도
    float maxSpeed;         // 최대 속도
    float acceleration;     // 가속도
    float scale;            // 크기 (아이템 효과용)
    bool shield;            // 보호막 (커지는 아이템 1회 충돌 무시용)
    float speedBoostTimer;  // 속도 아이템 지속시간
    float shrinkTimer;      // 축소 아이템 지속시간
    bool hasFinished;       // 통과 여부

    // 생성자
    Player(int pid = 0);

    // 효과 관련 메서드
    void ApplyItemEffect(ItemType itemType);    // 아이템 효과 적용
    void UpdateEffect(float deltaTime);    // 지속시간 관리
    void ResetEffect();                    // 효과 복원

private:
    // 원래 상태 저장용 (복원 시 필요)
    float baseScale = 1.0f;
    float baseMaxSpeed = 80.0f;
};
