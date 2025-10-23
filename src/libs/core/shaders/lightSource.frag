#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    float ambientFactor = 0.1;
    FragColor = vec4(objectColor * lightColor * ambientFactor, 1.0);
}