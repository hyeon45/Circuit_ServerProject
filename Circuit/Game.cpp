#include "Game.h"

// ���� ��� �ʱ�ȭ
Game* Game::instance = nullptr;

Game::Game() {
    instance = this;
}

// -------------------------
// ���α׷� ����
// -------------------------
void Game::Run(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(NULL)));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Circuit Game");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW �ʱ�ȭ ����\n";
        return;
    }

    Initialize();

    // GLUT �ݹ� ���
    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutKeyboardUpFunc(KeyboardUpCallback);
    glutSpecialFunc(SpecialKeyCallback);
    glutSpecialUpFunc(SpecialKeyUpCallback);
    glutTimerFunc(16, TimerCallback, 0);

    glutMainLoop();
}

// -------------------------
// �ʱ�ȭ
// -------------------------
void Game::Initialize() {
    renderer.Initialize();
    car.Init();
    obstacles.Init();
    items.Init();

    glEnable(GL_DEPTH_TEST);
    lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
}

// -------------------------
// �� ������ ������Ʈ
// -------------------------
void Game::Update(float dt) {
    // ����, ��ֹ�, ������ ����
    car.Update(dt);
    obstacles.Collision(car);
    items.Collision(car);

    if (gameRunning) {
        gameTime += dt;
    }
}

// -------------------------
// �׸���
// -------------------------
void Game::Draw() {
    renderer.DrawScene(car, obstacles, items);
}

// -------------------------
// GLUT �ݹ� ����
// -------------------------
void Game::DisplayCallback() {
    instance->Draw();
}

void Game::TimerCallback(int value) {
    float current = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float dt = current - instance->lastTime;
    instance->lastTime = current;

    if (dt > 0.1f) dt = 0.1f;

    instance->Update(dt);
    glutPostRedisplay();
    glutTimerFunc(16, TimerCallback, 0);
}

void Game::ReshapeCallback(int w, int h) {
    instance->windowWidth = w;
    instance->windowHeight = h;
    glViewport(0, 0, w, h);
}

void Game::KeyboardCallback(unsigned char key, int x, int y) {
    if (key == 27) glutLeaveMainLoop(); // ESC ����
    else if (key == 'r') instance->car.ResetToCheckpoint();
    else instance->car.HandleKeyboard(key, true);
}

void Game::KeyboardUpCallback(unsigned char key, int x, int y) {
    instance->car.HandleKeyboard(key, false);
}

void Game::SpecialKeyCallback(int key, int x, int y) {
    instance->car.HandleSpecialKey(key, true);
}

void Game::SpecialKeyUpCallback(int key, int x, int y) {
    instance->car.HandleSpecialKey(key, false);
}
