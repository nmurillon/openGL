#version 330 core

in float vertexColorFactor;

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(objectColor * lightColor * vertexColorFactor, 1.0);
}