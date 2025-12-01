#include "Game.h"

// 정적 멤버 초기화
Game* Game::instance = nullptr;

Game::Game() : playerID(-1) {
    instance = this;
}

// -------------------------
// 프로그램 실행
// -------------------------
void Game::Run(int argc, char** argv) {

    if (!networkManager.Connect()) {
        printf("서버 연결 실패...\n");
        return;   // 연결 안 되면 게임 실행 안 함
    }

    srand(static_cast<unsigned int>(time(NULL)));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Circuit Game");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW 초기화 실패\n";
        return;
    }

    Initialize();

    // GLUT 콜백 등록
    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutSpecialFunc(SpecialKeyCallback);
    glutSpecialUpFunc(SpecialKeyUpCallback);
    glutTimerFunc(16, TimerCallback, 0);

    glutMainLoop();
}

// -------------------------
// 차량 추가 및 초기화
// -------------------------
void Game::EnsureCarSlot(int id) {
    if (id < 0) return;

    if (id >= cars.size()) {
        size_t old = cars.size();
        cars.resize(id + 1);

        // 새로 생긴 Car는 Init() 필요
        for (size_t i = old; i < cars.size(); ++i) {
            cars[i].Init();
        }
    }
}


// -------------------------
// 초기화
// -------------------------
void Game::Initialize() {
    renderer.Initialize();
    cars.resize(2);   // 두 명 플레이어용
    for (auto& c : cars) c.Init();
    obstacles.Init();
    items.Init();
    networkManager.SetItemManager(&items);

    glEnable(GL_DEPTH_TEST);
    lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    // RecvThread 시작
    networkManager.StartRecvThread();
}

// -------------------------
// 매 프레임 업데이트
// -------------------------
void Game::Update(float dt) {

    // 1) 서버 목표 위치 쪽으로 렌더 위치를 한 번 당겨주기
    if (playerID >= 0 && playerID < cars.size()) {
        for (auto& c : cars) {
            /*std::cout << "playerID = " << playerID << std::endl;*/
            c.Update(dt);
        }
    }
    else {
        return;
    }

    uint8_t button = cars[playerID].GetInputMask();

    //if (playerID >= 0) { // 서버에서 start와 id 받으면 시작
    //    
    //}
    networkManager.SendCarMove(playerID, button);
   
    if (gameRunning) {
        gameTime += dt;
    }
}

// -------------------------
// 그리기
// -------------------------
void Game::Draw() {
    if (playerID >= 0 && playerID < cars.size())
        renderer.DrawScene(cars, playerID, obstacles, items);
}

// -------------------------
// GLUT 콜백 래퍼
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
    if (key == 27) glutLeaveMainLoop(); // ESC 종료
}


void Game::SpecialKeyCallback(int key, int x, int y) {
    if (instance->playerID >= 0 && instance->playerID < instance->cars.size()) {
        instance->cars[instance->playerID].HandleSpecialKey(key, true);
    }
}

void Game::SpecialKeyUpCallback(int key, int x, int y) {
    if (instance->playerID >= 0 && instance->playerID < instance->cars.size()) {
        instance->cars[instance->playerID].HandleSpecialKey(key, false);
    }
}

// -------------------------
// 서버에서 worldsync 받았을 때 호출해서 적용시킴
// -------------------------
void Game::OnWorldSync(const PKT_WorldSync& pkt) 
{
    //// 일단 내 플레이어만 반영
    //if (pkt.playerID != playerID) return;
    ////if (pkt.playerID == playerID)
    //    //std::cout << "x: " << pkt.posx << "," << "y: " << pkt.posy << "," << "z: " << pkt.posz << std::endl;
    //
    //car.SetPosition(pkt.posx, pkt.posy, pkt.posz);
    //car.SetYaw(pkt.yaw);
    //car.SetScale(pkt.scale);
    //car.SetShield(pkt.shield != 0);

    int id = pkt.playerID;

    // 혹시 서버가 먼저 보내는 상황을 대비
    /*std::cout << "playerID = " << playerID << std::endl;*/
    // std::cout << cars.size() << std::endl;

    cars[id].SetPosition(pkt.posx, pkt.posy, pkt.posz);
    cars[id].SetYaw(pkt.yaw);
    cars[id].SetScale(pkt.scale);
    cars[id].SetShield(pkt.shield != 0);
}