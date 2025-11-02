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
}
