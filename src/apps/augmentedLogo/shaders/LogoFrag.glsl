#version 330 core

in vec2 TexCoord;

uniform sampler2D iChannel0;

out vec4 FragColor;

void main()
{
    FragColor = vec4(texture(iChannel0, TexCoord).xyz, 0.5);
}