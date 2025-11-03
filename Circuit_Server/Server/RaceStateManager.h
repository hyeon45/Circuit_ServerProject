#pragma once
#include "WorldState.h"

// 레이스 도중 현재 상태에 대한 정의
enum class RaceState {
    WAITING,
    COUNTDOWN,
    PLAYING,
    FINISH
};

class RaceStateManager {
public:
    RaceStateManager();

    void StartCountdown();                // 경기 시작 전 카운트다운
    int  CheckFinishLine(WorldState& world); // 결승선 검사
    void EndRace(int winnerID);           // 레이스 종료

    RaceState GetState() const { return state; }

private:
    RaceState state;
    float countdownTimer;
    int winnerID;
    bool raceEnded;
};
