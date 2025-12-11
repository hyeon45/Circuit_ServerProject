#include "Game.h"
#include <GL/glu.h>

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
       // cars[playerID].Update(dt);
        for (auto& c : cars) {
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

    // 승자 결정시 결과 출력
    if (showResult) {
        int x = windowWidth / 2 - 60;   // 대충 가운데 쯤
        int y = windowHeight / 2;
        DrawText(resultText, x, y);

        // WinnerID도 표시
        char buf[64];
        snprintf(buf, sizeof(buf), "Winner ID: %d", winnerID_);
        DrawText(buf, x - 20, y - 40);
    }
    glutSwapBuffers();
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
    if (id >= cars.size()) {
        cars.resize(id);
    }

    cars[id].SetPosition(pkt.posx, pkt.posy, pkt.posz);
    cars[id].SetYaw(pkt.yaw);
    cars[id].SetScale(pkt.scale);
    cars[id].SetShield(pkt.shield != 0);
}

// Game Result 처리
void Game::ShowResult(int winnerID)
{
    winnerID_ = winnerID;
    showResult = true;
    //gameRunning = false;  // 필요시? 추가

    // 일단은 승자 ID 출력만
    printf("[RESULT] Winner ID = %d\n", winnerID);

    if (winnerID == playerID) {
        resultText = "YOU WIN!";
        printf("You Win!\n");
    }
    else {
        resultText = "YOU LOSE...";
        printf("You Lose... :(\n");
    }
    // 그와에 UI나 결과 화면 추가할지 정하기
}

// 2D 텍스트 그리기
void Game::DrawText(const std::string& text, int x, int y)
{
    // 3D용 셰이더 끄기
    glUseProgram(0);

    // 깊이 테스트 끄기 (UI가 위에 보이도록)
    glDisable(GL_DEPTH_TEST);

    // 투영 행렬 저장 & 2D 직교 투영으로 변경
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    // 모델뷰 행렬 저장 & 단위 행렬
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 텍스트 색 (흰색)
    glColor3f(1.0f, 1.0f, 1.0f);

    // 화면 좌표 (x, y) 에 텍스트 시작 위치 설정
    glRasterPos2i(x, y);

    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // 행렬 복구
    glPopMatrix(); // MODELVIEW
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // 다시 깊이 테스트 켜기
    glEnable(GL_DEPTH_TEST);
}