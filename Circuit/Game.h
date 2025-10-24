#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <iostream>
#include "Car.h"
#include "Obstacle.h"
#include "Item.h"
#include "Render.h"

class Game {
public:
    Game();
    void Run(int argc, char** argv);

private:
    // --- ��ü ---
    Renderer renderer;
    Car car;
    ObstacleManager obstacles;
    ItemManager items;

    // --- ���� ���� ---
    static Game* instance;
    int windowWidth = 1000;
    int windowHeight = 800;
    float lastTime = 0.0f;
    bool gameRunning = false;
    float gameTime = 0.0f;

    // --- ���� ���� ---
    void Initialize();
    void Update(float dt);
    void Draw();

    // --- GLUT �ݹ� (static wrapper) ---
    static void DisplayCallback();
    static void TimerCallback(int value);
    static void ReshapeCallback(int w, int h);
    static void KeyboardCallback(unsigned char key, int x, int y);
    static void SpecialKeyCallback(int key, int x, int y);
    static void SpecialKeyUpCallback(int key, int x, int y);
    static void KeyboardUpCallback(unsigned char key, int x, int y);
};
