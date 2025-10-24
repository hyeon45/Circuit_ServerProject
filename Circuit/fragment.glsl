#version 330 core

in vec2 TexCoord;                // vertex에서 전달받은 텍스처 좌표
out vec4 FragColor;

uniform sampler2D texture1;      // C++에서 연결된 텍스처

void main()
{
    FragColor = texture(texture1, TexCoord); // 실제 이미지 샘플링
}
