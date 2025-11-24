#pragma once
#include "WorldState.h"

class ScoreManager
{
public:
    ScoreManager(WorldState& world);

    // 승자 ID 반환 (0 또는 1, 아무도 없으면 -1)
    int RankPlayers();

private:
    WorldState& world;
};
