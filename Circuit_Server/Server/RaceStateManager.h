#pragma once
#include "WorldState.h"

class PacketHandler;    // 간접 접근

// 레이스 도중 현재 상태에 대한 정의
enum class RaceState {
    WAITING,
    COUNTDOWN,
    PLAYING,
    FINISH
};

class RaceStateManager {
public:
    RaceStateManager(PacketHandler* handler);

    void StartCountdown(float deltaTime);   // 경기 시작 전 카운트다운
    int  CheckFinishLine(WorldState& world); // 결승선 검사
    void EndRace(int winnerID);           // 레이스 종료
    void UpdateRaceTime(float deltaTime);   // 경기하는 총 시간 업데이트

    void SetState(RaceState s) { state = s; }   // 다른 파일에서도 설정을 할 수 있게끔.
    RaceState GetState() const { return state; }

private:
    RaceState state;
    PacketHandler* pkt_handler;
    
    // 카운트다운 시스템
    bool countdownStarted;
    float countdownTime;

    float totalRaceTime;    // 경기 누적 시간
    int winnerID;
    bool raceEnded;
};
