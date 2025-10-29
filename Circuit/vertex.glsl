#version 330 core

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 color;
layout (location = 2) in vec2 aTexCoord;   //  텍스처 좌표 입력

out vec2 TexCoord;                         // fragment로 전달
out vec3 VertexColor;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main()
{
    gl_Position = projectionTransform * viewTransform * modelTransform * vec4(aPos, 1.0);
    VertexColor = color;   // 색상정보 전달
    TexCoord = aTexCoord;  // UV 좌표 넘김
}
