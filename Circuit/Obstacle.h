#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <iostream>
#include "Car.h"

class ObstacleManager {
public:
    ObstacleManager();

    void Init();
    void Draw(GLuint shaderProgram) const;

private:
    GLuint vao, vbo, ebo;

    std::vector<glm::vec3> positions;
};
