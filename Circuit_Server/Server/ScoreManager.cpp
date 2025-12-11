#include "ScoreManager.h"

ScoreManager::ScoreManager(WorldState& world)
    : world(world)
{
}

// ---------------------------
// 승자 판정
// ---------------------------
int ScoreManager::RankPlayers()
{
    // 1) 두 플레이어가 모두 결승선 통과했는지 확인
    const auto& players = world.players;

    bool p0_finished = players[0].hasFinished;
    bool p1_finished = players[1].hasFinished;

    // 둘 다 안 끝났으면 승자 없음
    if (!p0_finished && !p1_finished)
        return -1;

    // 1명만 끝났으면 그 사람이 승자
    if (p0_finished && !p1_finished) return 0;
    if (!p0_finished && p1_finished) return 1;

    // 둘 다 끝났으면 시간 비교
    float t0 = players[0].finishTime;
    float t1 = players[1].finishTime;

    if (t0 < t1) return 0;
    else        return 1;
}
