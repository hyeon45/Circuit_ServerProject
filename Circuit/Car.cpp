#include "Car.h"
#include <GL/freeglut.h>

Car::Car()
    : position(100.0f, 10.0f, 1780.0f),
    yaw(0.0f),
    speed(0.0f),
    acceleration(40.0f),
    maxSpeed(80.0f),
    scale(1.0f),
    shield(false),
    movingForward(false),
    movingBackward(false),
    turningLeft(false),
    turningRight(false),
    speedBoostTimer(0.0f),
    shrinkTimer(0.0f),
    lastCheckpoint(0)
{
    checkpoints = {
        glm::vec3(100.0f,10.0f,1780.0f),
        glm::vec3(1000.0f,10.0f,1700.0f),
        glm::vec3(1050.0f,10.0f,1200.0f)
    };
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

void Car::Update(float dt) {
    // 속도 갱신
    if (movingForward) speed += acceleration * dt;
    else if (movingBackward) speed -= acceleration * dt;
    else {
        // 감속
        if (speed > 0.0f) speed -= acceleration * dt;
        else if (speed < 0.0f) speed += acceleration * dt;
    }

    if (speed > maxSpeed) speed = maxSpeed;
    if (speed < -maxSpeed) speed = -maxSpeed;

    Rotate(dt);
    Move(dt);

    // 아이템 효과 지속시간 갱신
    if (speedBoostTimer > 0.0f) {
        speedBoostTimer -= dt;
        if (speedBoostTimer <= 0.0f) maxSpeed /= 2.0f;
    }

    if (shrinkTimer > 0.0f) {
        shrinkTimer -= dt;
        if (shrinkTimer <= 0.0f) scale = 1.0f;
    }
}

void Car::Move(float dt) {
    glm::vec3 direction = glm::vec3(sin(yaw), 0.0f, -cos(yaw));
    position += direction * speed * dt;
}

void Car::Rotate(float dt) {
    float turnSpeed = glm::radians(15.0f);
    if (turningLeft) yaw -= turnSpeed * dt;
    if (turningRight) yaw += turnSpeed * dt;
}

void Car::Draw(GLuint shaderProgram) const {
    glUseProgram(shaderProgram);

    GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    if (objectColorLoc != -1) glUniform3f(objectColorLoc, 1.0f, 0.0f, 0.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, -yaw, glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(30.0f * scale, 5.0f * scale, 50.0f * scale));

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelTransform");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Car::HandleKeyboard(unsigned char key, bool pressed) {
    if (key == 'e' && pressed) ApplyItemEffect(static_cast<ItemType>(rand() % 3));
}

void Car::HandleSpecialKey(int key, bool pressed) {
    switch (key) {
    case GLUT_KEY_LEFT:  turningLeft = pressed; break;
    case GLUT_KEY_RIGHT: turningRight = pressed; break;
    case GLUT_KEY_UP:    movingForward = pressed; break;
    case GLUT_KEY_DOWN:  movingBackward = pressed; break;
    }
}

void Car::ApplyItemEffect(ItemType type) {
    switch (type) {
    case ItemType::GROW:
        scale = 2.0f;
        shield = true;
        break;
    case ItemType::SHRINK:
        scale = 0.5f;
        shrinkTimer = 5.0f;
        shield = false;
        break;
    case ItemType::SPEED_UP:
        maxSpeed *= 2.0f;
        speedBoostTimer = 5.0f;
        shield = false;
        break;
    }
}

void Car::ResetToCheckpoint() {
    if (!checkpoints.empty()) {
        position = checkpoints[lastCheckpoint];
        speed = 0.0f;
    }
}

void Car::SetScale(float newScale) {
    scale = newScale;
}

float Car::GetScale() const {
    return scale;
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