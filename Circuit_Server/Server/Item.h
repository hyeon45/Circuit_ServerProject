#pragma once

class Item
{
public:
    int id;
    float x, y, z;
    bool active;

    Item(int iid = 0, float px = 0.0f, float py = 0.0f, float pz = 0.0f)
        : id(iid), x(px), y(py), z(pz), active(true)
    {
    }
};
