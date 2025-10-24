#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "filetobuf.h"
#include "Car.h"
#include "Obstacle.h"
#include "Item.h"

class Renderer {
public:
    Renderer();
    void Initialize();
    void DrawScene(const Car& car, const ObstacleManager& obstacles, const ItemManager& items);

private:
    // Shader
    GLuint shaderProgramID;
    GLuint vertexShader;
    GLuint fragmentShader;

    // Texture
    GLuint textureMap;
    GLuint textureStar;

    // Floor
    GLuint vaoFloor, vboFloor, eboFloor;

    // 내부 헬퍼 함수
    void make_vertexShaders();
    void make_fragmentShaders();
    void make_shaderProgram();
    GLuint LoadTexture(const char* path);
    void InitFloorBuffer();
    void SetModelTransform(const glm::mat4& model);
};
