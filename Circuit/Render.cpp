#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Render.h"

// ------------------------
// 생성자
// ------------------------
Renderer::Renderer()
    : shaderProgramID(0), vertexShader(0), fragmentShader(0),
    textureMap(0), textureStar(0), vaoFloor(0), vboFloor(0), eboFloor(0) {
}

// ------------------------
// 셰이더 로딩
// ------------------------
void Renderer::make_vertexShaders() {
    GLchar* vertexSource = filetobuf("vertex.glsl");
    if (!vertexSource) exit(EXIT_FAILURE);
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
    glCompileShader(vertexShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        fprintf(stderr, "ERROR: vertex shader compile fail\n%s\n", errorLog);
        exit(EXIT_FAILURE);
    }
    free(vertexSource);
}

void Renderer::make_fragmentShaders() {
    GLchar* fragmentSource = filetobuf("fragment.glsl");
    if (!fragmentSource) exit(EXIT_FAILURE);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
    glCompileShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        fprintf(stderr, "ERROR: fragment shader compile fail\n%s\n", errorLog);
        exit(EXIT_FAILURE);
    }
    free(fragmentSource);
}

void Renderer::make_shaderProgram() {
    make_vertexShaders();
    make_fragmentShaders();

    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    GLint result;
    GLchar errorLog[512];
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, errorLog);
        fprintf(stderr, "ERROR: shader program link fail\n%s\n", errorLog);
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgramID);
}

// ------------------------
// 텍스처 로딩
// ------------------------
GLuint Renderer::LoadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = (nrComponents == 4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else {
        std::cerr << "Texture failed to load: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

// ------------------------
// 바닥 버퍼 초기화
// ------------------------
void Renderer::InitFloorBuffer() {
    GLfloat floorVertices[] = {
         0.0f,    0.0f,  2000.0f,   1.0f,0.0f,0.0f,   0.0f,0.0f, 0.0f,1.0f,0.0f,
         2000.0f, 0.0f,  2000.0f,   0.0f,1.0f,0.0f,   1.0f,0.0f, 0.0f,1.0f,0.0f,
         2000.0f, 0.0f,  0.0f,      0.0f,0.0f,1.0f,   1.0f,1.0f, 0.0f,1.0f,0.0f,
         0.0f,    0.0f,  0.0f,      1.0f,1.0f,0.0f,   0.0f,1.0f, 0.0f,1.0f,0.0f
    };

    GLuint floorIndices[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &vaoFloor);
    glBindVertexArray(vaoFloor);

    glGenBuffers(1, &vboFloor);
    glBindBuffer(GL_ARRAY_BUFFER, vboFloor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &eboFloor);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboFloor);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

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

// ------------------------
// 모델 행렬 전달
// ------------------------
void Renderer::SetModelTransform(const glm::mat4& model) {
    GLuint loc = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
}

// ------------------------
// 초기화
// ------------------------
void Renderer::Initialize() {
    make_shaderProgram();
    InitFloorBuffer();

    textureMap = LoadTexture("map.png");
    textureStar = LoadTexture("star.png");

    glUseProgram(shaderProgramID);
    glUniform1i(glGetUniformLocation(shaderProgramID, "texture1"), 0);

    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
}

// ------------------------
// 씬 렌더링
// ------------------------
void Renderer::DrawScene(const std::vector<Car>& cars, int playerID, const ObstacleManager& obstacles, const ItemManager& items)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramID);

    // ===========================
    // 카메라 내 플레이어 기준
    // ===========================
    glm::vec3 carPos = cars[playerID].GetPosition();
    glm::vec3 camPos = carPos + glm::vec3(
        -sin(cars[playerID].GetYaw()) * 100.0f,
        50.0f,
        cos(cars[playerID].GetYaw()) * 100.0f
    );

    glm::mat4 view = glm::lookAt(camPos, carPos, glm::vec3(0, 1, 0));
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1000.0f / 800.0f, 0.1f, 2000.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "viewTransform"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projectionTransform"), 1, GL_FALSE, &proj[0][0]);

    // ===========================
    // 바닥 렌더링
    // ===========================
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureMap);
    glUniform1i(glGetUniformLocation(shaderProgramID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgramID, "uUseTexture"), 1);

    glBindVertexArray(vaoFloor);
    SetModelTransform(glm::mat4(1.0f));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ===========================
    // 차량 + 장애물 + 아이템
    // ===========================
    glUniform1i(glGetUniformLocation(shaderProgramID, "uUseTexture"), 0);

    GLint colorLoc = glGetUniformLocation(shaderProgramID, "objectColor");

    for (int i = 0; i < cars.size(); i++) {
        if (i == playerID)
            glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);
        else
            glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);

        cars[i].Draw(shaderProgramID);
    }

    obstacles.Draw(shaderProgramID);
    items.Draw(shaderProgramID);

    //glutSwapBuffers();
}
