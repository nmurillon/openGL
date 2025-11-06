#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aNormal;

out float vertexColorFactor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientFactor = 0.1;

    vec3 pos = vec3(model * vec4(aPos, 1.0));

    vec3 lightDir = normalize(lightPos - pos);
    vec3 viewDir = normalize(viewPos - pos);
    vec3 reflected = reflect(-lightDir, aNormal);

    float diffuseFactor = max(dot(aNormal, lightDir), 0.0);
    float specularFactor = 0.5 * pow(max(dot(viewDir, reflected), 0.0), 32);
    
    vertexColorFactor = ambientFactor + diffuseFactor + specularFactor;
    gl_Position = projection * view * vec4(fragPos, 1.0);
}