#pragma once

enum class ItemType {
    None = 0,
    Grow = 1,       // 커지는 아이템
    Shrink = 2,     // 작아지는 아이템
    SpeedBoost = 3  // 속도 증가
};

class Item
{
public:
    int id;
    ItemType type;
    float x, y, z;
    bool active;

    Item(int iid = 0, float px = 0.0f, float py = 0.0f, float pz = 0.0f)
        : id(iid), x(px), y(py), z(pz), active(true)
    {
    }
};
