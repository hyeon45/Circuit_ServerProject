#include "Item.h"
#include <cstdlib>

// 서버에 저장

ItemManager::ItemManager() {
    item = {
        {0, {270.0f, 5.0f, 1100.0f}},
        {1, {300.0f, 5.0f, 1100.0f}},
        {2, {330.0f, 5.0f, 1100.0f}},
        {3, {360.0f, 5.0f, 1100.0f}},

        {4, {1000.0f, 5.0f, 1700.0f}},
        {5, {1000.0f, 5.0f, 1730.0f}},
        {6, {1000.0f, 5.0f, 1760.0f}},
        {7, {1000.0f, 5.0f, 1790.0f}},

        {8, {1840.0f, 5.0f, 900.0f}},
        {9, {1870.0f, 5.0f, 900.0f}},
        {10, {1900.0f, 5.0f, 900.0f}},
        {11, {1930.0f, 5.0f, 900.0f}},

        {12, {810.0f, 5.0f, 600.0f}},
        {13, {840.0f, 5.0f, 600.0f}},
        {14, {870.0f, 5.0f, 600.0f}},
        {15, {900.0f, 5.0f, 600.0f}},
    };
}

void ItemManager::Init() {
    GLfloat vertices[] = {
        10.0f,  10.0f,  10.0f,   0,0,1, 1,1, 0,1,0,
        10.0f,  10.0f, -10.0f,   0,0,1, 1,0, 0,1,0,
       -10.0f,  10.0f, -10.0f,   0,0,1, 0,0, 0,1,0,
       -10.0f,  10.0f,  10.0f,   0,0,1, 0,1, 0,1,0,
        10.0f, -10.0f,  10.0f,   0,0,1, 1,1, 0,-1,0,
        10.0f, -10.0f, -10.0f,   0,0,1, 1,0, 0,-1,0,
       -10.0f, -10.0f, -10.0f,   0,0,1, 0,0, 0,-1,0,
       -10.0f, -10.0f,  10.0f,   0,0,1, 0,1, 0,-1,0
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

void ItemManager::Draw(GLuint shaderProgram) const {
    glUseProgram(shaderProgram);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(colorLoc, 0.0f, 1.0f, 1.0f);

    glBindVertexArray(vao);
    for (const auto& pos : item) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos.pos);
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelTransform");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

void ItemManager::RemoveItem(int itemID) {
    item.erase(
        std::remove_if(item.begin(), item.end(),
            [&](const ItemData& item) {
                return item.id == itemID;
            }),
        item.end()
    );
}
