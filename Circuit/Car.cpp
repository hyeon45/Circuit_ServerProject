#include "Car.h"
#include <GL/freeglut.h>

Car::Car()
    : serverPosition(100.0f, 10.0f, 1780.0f),
    renderPosition(serverPosition),
    serverYaw(0.0f),
    renderYaw(0.0f),
    scale(1.0f),
    shield(false)
{

}

void Car::Init() {
    // 차량 도형 (직육면체 형태)
    GLfloat vertices[] = {
         0.4f,  0.5f,  0.1f,   1,0,0, 1,1, 0,1,0,
         0.4f,  0.5f, -0.1f,   1,0,0, 1,0, 0,1,0,
        -0.4f,  0.5f, -0.1f,   1,0,0, 0,0, 0,1,0,
        -0.4f,  0.5f,  0.1f,   1,0,0, 0,1, 0,1,0,

         0.4f, -0.5f,  0.1f,   1,0,0, 1,1, 0,-1,0,
         0.4f, -0.5f, -0.1f,   1,0,0, 1,0, 0,-1,0,
        -0.4f, -0.5f, -0.1f,   1,0,0, 0,0, 0,-1,0,
        -0.4f, -0.5f,  0.1f,   1,0,0, 0,1, 0,-1,0
    };

    GLuint indices[] = {
        0,1,2, 0,2,3,
        4,5,6, 4,6,7,
        0,3,7, 0,7,4,
        1,2,6, 1,6,5,
        2,3,7, 2,7,6,
        0,1,5, 0,5,4
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

void Car::Draw(GLuint shaderProgram) const {
    glUseProgram(shaderProgram);

    GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    if (objectColorLoc != -1) glUniform3f(objectColorLoc, 1.0f, 0.0f, 0.0f);

    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 pos = renderPosition;
    model = glm::translate(model, pos);
    model = glm::rotate(model, -renderYaw, glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(30.0f * scale, 5.0f * scale, 50.0f * scale));

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelTransform");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Car::HandleSpecialKey(int key, bool pressed) {
    switch (key) {
    case GLUT_KEY_LEFT:  turningLeft = pressed; break;
    case GLUT_KEY_RIGHT: turningRight = pressed; break;
    case GLUT_KEY_UP:    movingForward = pressed; break;
    case GLUT_KEY_DOWN:  movingBackward = pressed; break;
    }
}

//-------------------------------
// 현재 입력을 mask하기 
//-------------------------------
uint8_t Car::GetInputMask() const
{
    uint8_t mask = 0;

    if (movingForward)  mask |= 0x01;
    if (movingBackward) mask |= 0x02;
    if (turningLeft)    mask |= 0x04;
    if (turningRight)   mask |= 0x08;

    return mask;
}

//-------------------------------
// 서버에서 받은 입력 적용
//-------------------------------
void Car::SetPosition(float x, float y, float z) 
{
    serverPosition.x = x;
    serverPosition.y = y;
    serverPosition.z = z;
    // std::cout << "x: " << x << "," << "y: " << y << "," << "z: " << z << std::endl;
}

void Car::SetYaw(float newYaw) 
{
    serverYaw = newYaw;
}

void Car::SetShield(bool active) 
{
    shield = active;
}

void Car::SetScale(float newScale) {
    scale = newScale;
}

void Car::Update(float dt)
{
    // 부드러움 정도 조절용 상수 (값 키우면 더 빨리 따라감)
    const float smooth = 10.0f;

    // 위치 보간: renderPos가 serverPos 쪽으로 천천히 이동
    float alpha = smooth * dt;
    if (alpha > 1.0f) alpha = 1.0f;

    renderPosition = glm::mix(renderPosition, serverPosition, alpha);

    // yaw도 비슷하게 (단순 보간, 각도 wrap 처리는 나중에 필요하면 추가)
    renderYaw = renderYaw + (serverYaw - renderYaw) * alpha;
}