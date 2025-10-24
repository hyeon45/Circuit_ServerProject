#version 330 core

in vec2 TexCoord;                // vertex���� ���޹��� �ؽ�ó ��ǥ
out vec4 FragColor;

uniform sampler2D texture1;      // C++���� ����� �ؽ�ó

void main()
{
    FragColor = texture(texture1, TexCoord); // ���� �̹��� ���ø�
}
