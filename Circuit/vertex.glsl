#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;   //  텍스처 좌표 입력

out vec2 TexCoord;                         // fragment로 전달

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main()
{
    gl_Position = projectionTransform * viewTransform * modelTransform * vec4(aPos, 1.0);
    TexCoord = aTexCoord;  // UV 좌표 넘김
}
