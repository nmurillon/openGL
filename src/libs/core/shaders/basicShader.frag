#version 330 core

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float TextureMixingOpacity;

out vec4 FragColor;

void main()
{
    FragColor = mix(texture(Texture1, TexCoord), texture(Texture2, vec2(1-TexCoord.x, TexCoord.y)), TextureMixingOpacity);
}