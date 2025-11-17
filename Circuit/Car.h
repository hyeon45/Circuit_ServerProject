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
    void HandleKeyboard(unsigned char key, bool pressed);
    void HandleSpecialKey(int key, bool pressed);

    // 아이템 관련
    void ApplyItemEffect(ItemType type);
    void ResetToCheckpoint();

    // Getter
    glm::vec3 GetPosition() const { return position; }
    float GetYaw() const { return yaw; }

    bool IsShieldActive() const { return shield; }
    void DisableShield() { shield = false; }
    void StopMovement() { speed = 0.0f; }

    void SetScale(float newScale);
    float GetScale() const;

    // 서버에서 받은 값 set 해주기
    void SetPosition(float x, float y, float z);
    void SetYaw(float newYaw);
    void SetShield(bool active);

    // 현재 입력 상태 체크
    uint8_t GetInputMask() const;

private:
    // 차량 상태
    glm::vec3 position;
    float yaw;
    float speed;
    float acceleration;
    float maxSpeed;
    float scale;
    bool shield;

    // 키 입력 상태
    bool movingForward;
    bool movingBackward;
    bool turningLeft;
    bool turningRight;

    // 아이템 효과 지속 시간
    float speedBoostTimer;
    float shrinkTimer;

    // 렌더링용 버퍼
    GLuint vao, vbo, ebo;

    // 체크포인트
    std::vector<glm::vec3> checkpoints;
    int lastCheckpoint;

    // 내부 함수
    void Move(float dt);
    void Rotate(float dt);
};
