#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D tex;
uniform bool showDepthBuffer;

// TODO: set these dynamically
float near = 0.1; 
float far  = 100.0;

void main()
{
    if (showDepthBuffer) {
        // Get normalized device coordinates
        float ndc = 2.0 * gl_FragCoord.z - 1.0; 
        float linearDepth = near * (ndc + 1.0) / (far + near - ndc * (far - near));
        fragColor = vec4(vec3(linearDepth), 1.0);
    } else {
        fragColor = texture(tex, texCoords);
    }
}