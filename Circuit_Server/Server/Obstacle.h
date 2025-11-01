#pragma once

class Obstacle
{
public:
    int id;
    float x, y, z;  // 중심 좌표
    float radius;   // 충돌 반경

    Obstacle(int oid = 0, float px = 0.0f, float py = 0.0f, float pz = 0.0f, float r = 20.0f)
        : id(oid), x(px), y(py), z(pz), radius(r) {
    }
};
