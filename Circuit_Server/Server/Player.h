#pragma once
#include <cmath>

class Player
{
public:
    int id;           // �÷��̾� ID
    float x, y, z;    // ��ġ
    float yaw;        // ȸ��(����)
    float speed;      // ���� �ӵ�
    float maxSpeed;   // �ִ� �ӵ�
    float acceleration; // ���ӵ�
    float scale;      // ũ��(������ ȿ����)
    bool shield;      // ��ȣ�� ����
    float speedBoostTimer; // �ӵ� ������ ���ӽð�
    float shrinkTimer;     // ��� ������ ���ӽð�

    Player(int pid = 0);

    void ApplyItemEffect(int type); // ������ ȿ�� ����
    void Update(float dt);          // �̵� ���� (yaw ���)
};
