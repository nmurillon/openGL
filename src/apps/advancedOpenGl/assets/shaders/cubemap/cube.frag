#version 330 core

out vec4 fragColor;

in vec3 pos;
in vec3 normal;

uniform vec3 camPos;
uniform samplerCube cubemap;

void main()
{
    vec3 incidence = normalize(pos - camPos);
    vec3 reflected = reflect(incidence, normalize(normal));
    fragColor = texture(cubemap, reflected);
}