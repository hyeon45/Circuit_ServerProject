#include "Item.h"
#include <cstdlib>

ItemManager::ItemManager() {
    positions = {
        {270.0f, 5.0f, 1100.0f},
        {300.0f, 5.0f, 1100.0f},
        {330.0f, 5.0f, 1100.0f},
        {360.0f, 5.0f, 1100.0f},
        {1000.0f, 5.0f, 1700.0f},
        {1000.0f, 5.0f, 1730.0f},
        {1000.0f, 5.0f, 1760.0f},
        {1000.0f, 5.0f, 1790.0f},
        {1840.0f, 5.0f, 900.0f},
        {1870.0f, 5.0f, 900.0f},
        {1900.0f, 5.0f, 900.0f},
        {1930.0f, 5.0f, 900.0f},
        {810.0f, 5.0f, 600.0f},
        {840.0f, 5.0f, 600.0f},
        {870.0f, 5.0f, 600.0f},
        {900.0f, 5.0f, 600.0f}
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
    for (const auto& pos : positions) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelTransform");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

void ItemManager::Collision(Car& car) {
    const float collisionRange = 15.0f;

    for (size_t i = 0; i < positions.size();) {
        float dist = glm::distance(car.GetPosition(), positions[i]);
        if (dist < collisionRange) {
            ItemType randomItem = static_cast<ItemType>(rand() % 3);
            car.ApplyItemEffect(randomItem);

            std::cout << "¾ÆÀÌÅÛ È¹µæ! È¿°ú Àû¿ëµÊ." << std::endl;
            RemoveItem(i);
        }
        else {
            ++i;
        }
    }
}

void ItemManager::RemoveItem(size_t index) {
    if (index < positions.size())
        positions.erase(positions.begin() + index);
}
