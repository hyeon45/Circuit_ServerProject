#version 330 core

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 color;
layout (location = 2) in vec2 aTexCoord;   //  �ؽ�ó ��ǥ �Է�

out vec2 TexCoord;                         // fragment�� ����
out vec3 VertexColor;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main()
{
    gl_Position = projectionTransform * viewTransform * modelTransform * vec4(aPos, 1.0);
    VertexColor = color;   // �������� ����
    TexCoord = aTexCoord;  // UV ��ǥ �ѱ�
}
