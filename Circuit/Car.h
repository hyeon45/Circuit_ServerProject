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

    // �Է� ó��
    void HandleKeyboard(unsigned char key, bool pressed);
    void HandleSpecialKey(int key, bool pressed);

    // ������ ����
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

private:
    // ���� ����
    glm::vec3 position;
    float yaw;
    float speed;
    float acceleration;
    float maxSpeed;
    float scale;
    bool shield;

    // Ű �Է� ����
    bool movingForward;
    bool movingBackward;
    bool turningLeft;
    bool turningRight;

    // ������ ȿ�� ���� �ð�
    float speedBoostTimer;
    float shrinkTimer;

    // �������� ����
    GLuint vao, vbo, ebo;

    // üũ����Ʈ
    std::vector<glm::vec3> checkpoints;
    int lastCheckpoint;

    // ���� �Լ�
    void Move(float dt);
    void Rotate(float dt);
};
