#version 330 core

in vec2 TexCoord;                // vertex���� ���޹��� �ؽ�ó ��ǥ
in vec3 VertexColor;

out vec4 FragColor;

uniform sampler2D texture1;      // C++���� ����� �ؽ�ó
uniform int uUseTexture;

void main()
{
    if(uUseTexture == 1){
        FragColor = texture(texture1, TexCoord); // ���� �̹��� ���ø�
    }else{
        FragColor = vec4(VertexColor, 1.0); 
    }
}
