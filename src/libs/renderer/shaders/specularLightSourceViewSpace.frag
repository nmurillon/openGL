#version 330 core

in vec3 normal;
in vec3 fragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
in vec3 lightPos;

out vec4 FragColor;

void main()
{
    float ambientFactor = 0.1;
    float specularFactor = 0.5;

    vec3 norm = normalize(normal);

    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(-fragPos);
    vec3 reflected = reflect(-lightDir, norm);

    float diffuseFactor = max(dot(norm, lightDir), 0.0);
    // Putting the result of the dot product to a given power defines the shininess of the light.
    // A higher value means the light is properly reflected and not scattered around.
    specularFactor = 0.5 * pow(max(dot(viewDir, reflected), 0.0), 32);
    
    FragColor = vec4(objectColor * lightColor * (specularFactor + diffuseFactor + ambientFactor), 1.0);
}