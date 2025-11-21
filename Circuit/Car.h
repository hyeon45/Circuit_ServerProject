#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <iostream>

enum class ItemType {
    GROW,
    SHRINK,
    SPEED_UP
};

class Car {
public:
    Car();

    void Init();
    void Update(float dt);
    void Draw(GLuint shaderProgram) const;

    // 입력 처리
    void HandleSpecialKey(int key, bool pressed);

    // Getter
    glm::vec3 GetPosition() const { return renderPosition; }
    float GetYaw() const { return renderYaw; }

    bool IsShieldActive() const { return shield; }
    void DisableShield() { shield = false; }
    void StopMovement() { speed = 0.0f; }

    void SetScale(float newScale);

    // 서버에서 받은 값 set 해주기
    void SetPosition(float x, float y, float z);
    void SetYaw(float newYaw);
    void SetShield(bool active);

    // 현재 입력 상태 체크
    uint8_t GetInputMask() const;

private:
    // 차량 상태
    glm::vec3 serverPosition;  // 서버에서 받은 실제 위치
    glm::vec3 renderPosition;  // 화면에 그릴 위치

    float serverYaw;
    float renderYaw;

    //float yaw;
    float speed;
    float acceleration;
    float maxSpeed;
    float scale;
    bool shield;

    // 키 입력 상태
    bool movingForward = false;
    bool movingBackward = false;
    bool turningLeft = false;
    bool turningRight = false;

    // 아이템 효과 지속 시간
    float speedBoostTimer;
    float shrinkTimer;

    // 렌더링용 버퍼
    GLuint vao, vbo, ebo;
};
