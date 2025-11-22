#include "RaceStateManager.h"
#include <iostream>
#include "PacketHandler.h"

RaceStateManager::RaceStateManager(PacketHandler* handler)
    : pkt_handler(handler), 
    state(RaceState::WAITING),
    countdownStarted(false),
    countdownTime(0.0f),
    winnerID(-1),
    raceEnded(false)
{
}

// ---------------------------------------------
// 1. 카운트다운 시작
// ---------------------------------------------
void RaceStateManager::StartCountdown(float deltaTime) {
    if (state != RaceState::COUNTDOWN)
        return;

    // 첫 진입
    if (!countdownStarted)
    {
        countdownStarted = true;
        countdownTime = 0.0f;
        printf("[Race] COUNTDOWN START!\n");
    }

    countdownTime += deltaTime;

    if (countdownTime >= 3.0f)
    {

        pkt_handler->SendGameStart();

        state = RaceState::PLAYING;
        printf("[Race] GAME START!\n");
    }
}

// ---------------------------------------------
// 2. 결승선 통과 여부 검사
// ---------------------------------------------
int RaceStateManager::CheckFinishLine(WorldState& world) {
    if (state != RaceState::PLAYING || raceEnded)
        return -1;

    for (auto& player : world.players)
    {
        float x = player.x;
        float z = player.z;
        
        // std::cout << world.finishTrigger.IsInside(x, z) << std::endl;

        // FinishLineTrigger 사용하여 플레이어가 결승선을 통과헀는지에 대한 여부 판단.
        if (world.finishTrigger.IsInside(x, z))
        {
            player.hasFinished = true;
            winnerID = player.id;
            raceEnded = true;
            state = RaceState::FINISH;

            printf("[Race] Player %d reached the finish!\n", winnerID);
            return winnerID;
        }
    }

    return -1;
}

// ---------------------------------------------
// 3. 경기 종료 처리 (임시)
// ---------------------------------------------
void RaceStateManager::EndRace(int winnerID) {
    raceEnded = true;
    state = RaceState::FINISH;

    // 다음 경기 준비
    countdownStarted = false;
    countdownTime = 0.0f;
    this->winnerID = winnerID;


}
