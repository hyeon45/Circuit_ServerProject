#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <iostream>
#include "Car.h"

class ItemManager {
public:
    // ItemManager();

    void Init();
    void Draw(GLuint shaderProgram) const;
    void Collision(Car& car);

private:
    GLuint vao, vbo, ebo;
    std::vector<glm::vec3> positions;

    void RemoveItem(size_t index);
};
