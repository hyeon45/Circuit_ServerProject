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
    

}

// ---------------------------------------------
// 3. 경기 종료 처리
// ---------------------------------------------
void RaceStateManager::EndRace(int winnerID) {
    


}
