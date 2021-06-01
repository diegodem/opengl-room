#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 Tex;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

void main()
{
    FragColor = texture(texture1, TexCoord) * Tex.x + texture(texture2, TexCoord) * Tex.y + texture(texture3, TexCoord) * Tex.z;

}