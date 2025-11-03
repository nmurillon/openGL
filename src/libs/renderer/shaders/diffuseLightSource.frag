#version 330 core

in vec3 normal;
in vec3 fragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; 

out vec4 FragColor;

void main()
{
    float ambientFactor = 0.1;
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuseFactor = max(dot(normalize(normal), lightDir), 0.0);
    FragColor = vec4(objectColor * lightColor * (diffuseFactor + ambientFactor), 1.0);
}