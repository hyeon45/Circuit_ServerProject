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
    void Collision(Car& car);

private:
    GLuint vao, vbo, ebo;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec4> bounds; // (minX, maxX, minZ, maxZ)

    void RemoveObstacle(size_t index);
};
