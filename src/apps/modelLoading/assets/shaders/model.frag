#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outerCutoff;
};

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform Material material;
uniform DirectionalLight directionalLight;

// We can not define a dynamic number of lights
#define N_POINT_LIGHTS 4
uniform PointLight[N_POINT_LIGHTS] pointLights;
uniform int pointLightCount;

uniform vec3 viewPos;

out vec4 FragColor;

vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    vec3 reflected = reflect(-lightDir, normal);

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 diffuse = light.diffuse * max(dot(normal, lightDir), 0.0) * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 specular = light.specular * vec3(texture(material.texture_specular1, texCoords)) * pow(max(dot(viewDir, reflected), 0.0), material.shininess);

    return ambient + diffuse + specular;
}

vec3 computePointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflected = reflect(-lightDir, normal);

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 diffuse = light.diffuse * max(dot(normal, lightDir), 0.0) * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 specular = light.specular * vec3(texture(material.texture_specular1, texCoords)) * pow(max(dot(viewDir, reflected), 0.0), material.shininess);

    float dist = distance(fragPos, light.position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    
    return attenuation * (specular + diffuse + ambient);
}

void main()
{
    vec3 normal = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    //vec3 directional = computeDirectionalLight(directionalLight, normal, viewDir);
    vec3 points = vec3(0.0);

    //for (int i = 0; i < pointLightCount; ++i) {
    //    points += computePointLight(pointLights[i], normal, viewDir);
    //}
    
    //FragColor = vec4(directional + points, 1.0);
    FragColor = texture(material.texture_diffuse1, texCoords);
}