#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 color;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec3 fragPos;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(normal);

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflected = reflect(-lightDir, norm);

    vec3 diffuse = max(dot(norm, lightDir), 0.0) * material.diffuse;
    vec3 specular = material.diffuse * pow(max(dot(viewDir, reflected), 0.0), material.shininess);
    
    FragColor = vec4(light.color * (specular + diffuse + material.ambient), 1.0);
}