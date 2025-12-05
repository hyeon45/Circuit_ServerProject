#version 330 core

in vec2 TexCoord;                // vertex에서 전달받은 텍스처 좌표
in vec3 VertexColor;

out vec4 FragColor;

uniform sampler2D texture1;      // C++에서 연결된 텍스처
uniform int uUseTexture;

void main()
{
    if(uUseTexture == 1){
        FragColor = texture(texture1, TexCoord); // 실제 이미지 샘플링
    }else{
        FragColor = vec4(VertexColor, 1.0); 
    }
}
