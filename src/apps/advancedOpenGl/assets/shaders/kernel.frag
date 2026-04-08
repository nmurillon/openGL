#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;

uniform float[9] kernel;

const float offset = 1.0 / 300.0;

void main()
{

    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), // top left
        vec2(0.0, offset),
        vec2(offset, offset), // top right
        vec2(-offset, 0.0),
        vec2(0.0, 0.0),
        vec2(offset, 0.0),
        vec2(-offset, -offset),
        vec2(0.0, -offset),
        vec2(offset, -offset)
    );

    vec3 sampleTexture[9];
    for (int i = 0; i < 9; ++i) {
        sampleTexture[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
    }

    vec3 color = vec3(0.0);

    for (int i = 0; i < 9; ++i) {
        color += sampleTexture[i] * kernel[i];
    } 

    FragColor = vec4(color, 1.0);
}