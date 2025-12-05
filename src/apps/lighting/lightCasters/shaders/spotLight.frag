#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
};

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(normal);

    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    float inSpotLight = step(light.cutoff, theta);


    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflected = reflect(-lightDir, norm);

    // In case we are out of the spotlight, use ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = light.diffuse * max(dot(norm, lightDir), 0.0) * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * vec3(texture(material.specular, texCoords)) * pow(max(dot(viewDir, reflected), 0.0), material.shininess);

    float dist = distance(fragPos, light.position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec3 lightInsideSpotlight = inSpotLight * (attenuation* (specular + diffuse) + ambient);
    vec3 lightOutsideSpotlight = (1.0 - inSpotLight) * ambient;
    
    FragColor = vec4(lightInsideSpotlight + lightOutsideSpotlight, 1.0);
}