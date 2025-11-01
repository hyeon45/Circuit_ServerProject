#pragma once

class Obstacle
{
public:
    int id;
    float x, y, z;  // �߽� ��ǥ
    float radius;   // �浹 �ݰ�

    Obstacle(int oid = 0, float px = 0.0f, float py = 0.0f, float pz = 0.0f, float r = 20.0f)
        : id(oid), x(px), y(py), z(pz), radius(r) {
    }
};
