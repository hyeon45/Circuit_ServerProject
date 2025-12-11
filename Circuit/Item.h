#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <iostream>
#include "Car.h"

struct ItemData {
    int id;
    glm::vec3 pos;
};

class ItemManager {
public:
     ItemManager();

    void Init();
    void Draw(GLuint shaderProgram) const;
    void ApplyItemDelete(int itemID); // itemID∑Œ ªË¡¶

    std::vector<ItemData> item;

private:
    GLuint vao, vbo, ebo;
};
