#include "RaceStateManager.h"
#include <iostream>

RaceStateManager::RaceStateManager()
    : state(RaceState::WAITING),
    countdownTimer(3.0f),
    winnerID(-1),
    raceEnded(false)
{
}

// ---------------------------------------------
// 1. 카운트다운 시작
// ---------------------------------------------
void RaceStateManager::StartCountdown() {


}

// ---------------------------------------------
// 2. 결승선 통과 여부 검사
// ---------------------------------------------
int RaceStateManager::CheckFinishLine(WorldState& world) {
    if (state != RaceState::PLAYING || raceEnded)
        return -1;

    // 결승선 범위 (맵 구조 기준)
    const float finishZMin = 1440.0f;
    const float finishZMax = 1540.0f;
    const float finishXMin = 1000.0f;
    const float finishXMax = 1140.0f;

    for (auto& player : world.players)
    {
        float x = player.position.x;
        float z = player.position.z;

        // 결승선 통과 검사
        if (x >= finishXMin && x <= finishXMax &&
            z >= finishZMin && z <= finishZMax &&
            !player.hasFinished)
        {
            player.hasFinished = true;
            winnerID = player.id;
            state = RaceState::FINISH;
            raceEnded = true;

            std::cout << "[Race] Player " << winnerID << " reached the finish line!\n";
            return winnerID;
        }
    }

    return -1; // 아직 통과 없음
}

// ---------------------------------------------
// 3. 경기 종료 처리
// ---------------------------------------------
void RaceStateManager::EndRace(int winnerID) {
    


}
