#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <iostream>
#include "Car.h"
#include "Obstacle.h"
#include "Item.h"
#include "Render.h"
#include "NetworkManager.h"
#include <string>

class Game {
public:
    Game();   // 처음엔 미할당 상태
    static Game* GetInstance() { return instance; }
    void Run(int argc, char** argv);

    // 서버에서 playerid 받아서 저장
    void SetPlayerID(int id) { playerID = id; EnsureCarSlot(id);  std::cout << id << std::endl; }
    int  GetPlayerID() const { return playerID; }

    // 서버에서 WorldSync를 받았을 때 호출될 함수
    void OnWorldSync(const PKT_WorldSync& pkt);

    // 서버에서 받은 GameResult 처리
    void ShowResult(int winnerID);

private:
    int playerID;

    // --- 객체 ---
    Renderer renderer;
    std::vector<Car> cars;
    ObstacleManager obstacles;
    ItemManager items;
    NetworkManager networkManager;

    // --- 상태 변수 ---
    static Game* instance;
    int windowWidth = 1000;
    int windowHeight = 800;
    float lastTime = 0.0f;
    bool gameRunning = false;
    float gameTime = 0.0f;

    // ==== 결과 UI용 ====
    bool showResult = false;
    int winnerID_ = -1;
    std::string resultText;

    // --- 내부 로직 ---
    void EnsureCarSlot(int id);
    void Initialize();
    void Update(float dt);
    void Draw();
    void DrawText(const std::string& text, int x, int y);  // UI 그리기 용

    // --- GLUT 콜백 (static wrapper) ---
    static void DisplayCallback();
    static void TimerCallback(int value);
    static void ReshapeCallback(int w, int h);
    static void KeyboardCallback(unsigned char key, int x, int y);
    static void SpecialKeyCallback(int key, int x, int y);
    static void SpecialKeyUpCallback(int key, int x, int y);
};
