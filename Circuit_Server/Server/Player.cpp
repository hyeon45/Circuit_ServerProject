#include "Player.h"

Player::Player(int pid)
    : id(pid),
    x(0.0f), y(0.0f), z(0.0f),
    yaw(0.0f),
    speed(0.0f),
    maxSpeed(80.0f),
    acceleration(40.0f),
    scale(1.0f),
    shield(false),
    speedBoostTimer(0.0f),
    shrinkTimer(0.0f)
{
    baseScale = scale;
    baseMaxSpeed = maxSpeed;
}

// -----------------------------------------
// 아이템 효과 적용
// -----------------------------------------
void Player::ApplyItemEffect(ItemType type) {
    switch (type) {
    case ItemType::Grow:
        scale *= 2.0f;
        shield = true;
        break;
    case ItemType::Shrink:
        scale *= 0.5f;
        shrinkTimer = 5.0f;
        break;
    case ItemType::SpeedBoost:
        maxSpeed *= 2.0f;
        speedBoostTimer = 3.0f;
        break;
    default:
        break;
    }
}


// -----------------------------------------
// 지속시간 업데이트 (PhysicsThread에서 매 tick마다 호출)
// -----------------------------------------
void Player::UpdateEffect(float deltaTime)
{
    // 속도 증가 효과
    if (speedBoostTimer > 0.0f)
    {
        speedBoostTimer -= deltaTime;
        if (speedBoostTimer <= 0.0f)
        {
            maxSpeed = baseMaxSpeed;
        }
    }

    // 축소 효과
    if (shrinkTimer > 0.0f)
    {
        shrinkTimer -= deltaTime;
        if (shrinkTimer <= 0.0f)
        {
            scale = baseScale;
        }
    }

    // 커지는 아이템(보호막)은 타이머가 없고, 1회 충돌 시 해제됨
}

// -----------------------------------------
// 전체 리셋 (다음 라운드 시작 시)
// -----------------------------------------
void Player::ResetEffect()
{
    scale = baseScale;
    maxSpeed = baseMaxSpeed;
    shield = false;
    speedBoostTimer = 0.0f;
    shrinkTimer = 0.0f;
}
