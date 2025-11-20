#include "Obstacle.h"
#include "Car.h"

ObstacleManager::ObstacleManager() {
    // 장애물 위치 목록
    // 서버에 저장
    
    positions = {
        {60.0f, 10.1f, 1500.0f},
        {140.0f,10.1f,1200.0f},
        {60.0f,10.1f,900.0f},
        {280.0f,10.1f,900.0f},
        {280.0f,10.1f,1200.0f},
        {360.0f,10.1f,1600.0f},
        {500.0f,10.1f,1700.0f},
        {800.0f,10.1f,1760.0f},
        {1200.0f,10.1f,1700.0f},
        {1500.0f,10.1f,1760.0f},
        {1920.0f,10.1f,1400.0f},
        {1920.0f,10.1f,1100.0f},
        {1860.0f,10.1f,800.0f},
        {1240.0f,10.1f,800.0f},
        {1180.0f,10.1f,1100.0f},
        {820.0f,10.1f,900.0f},
        {900.0f,10.1f,500.0f},
        {600.0f,10.1f,700.0f},
        {660.0f,10.1f,1000.0f},
        {860.0f,10.1f,1520.0f}
    };
}

// -----------------------------
// VAO / VBO 초기화
// -----------------------------
void ObstacleManager::Init() {
    GLfloat vertices[] = {
        20.0f,  10.0f,  20.0f,   1,1,0, 1,1, 0,1,0,
        20.0f,  10.0f, -20.0f,   1,1,0, 1,0, 0,1,0,
       -20.0f,  10.0f, -20.0f,   1,1,0, 0,0, 0,1,0,
       -20.0f,  10.0f,  20.0f,   1,1,0, 0,1, 0,1,0,
        20.0f, -10.0f,  20.0f,   1,1,0, 1,1, 0,-1,0,
        20.0f, -10.0f, -20.0f,   1,1,0, 1,0, 0,-1,0,
       -20.0f, -10.0f, -20.0f,   1,1,0, 0,0, 0,-1,0,
       -20.0f, -10.0f,  20.0f,   1,1,0, 0,1, 0,-1,0
    };

    GLuint indices[] = {
        0,1,2, 0,2,3,
        4,5,6, 4,6,7,
        0,3,7, 0,7,4,
        1,2,6, 1,6,5,
        2,3,7, 2,7,6,
        0,1,5, 0,5,4
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

// -----------------------------
// 장애물 그리기
// -----------------------------
void ObstacleManager::Draw(GLuint shaderProgram) const {
    glUseProgram(shaderProgram);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f);

    glBindVertexArray(vao);
    for (const auto& pos : positions) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelTransform");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}
