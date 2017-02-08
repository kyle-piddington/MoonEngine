#version 400 core
in vec3 fragPos;
in vec3 fragNor;
in vec2 fragTex;

layout (location = 0) out vec3 posOut;
layout (location = 1) out vec3 colorOut;
layout (location = 2) out vec3 normalOut;
layout (location = 3) out vec3 textureOut;

uniform sampler2D diffuse;

void main()
{
    posOut = fragPos;
    colorOut = texture(diffuse, fragTex).rgb;
    normalOut = normalize(fragNor);
    textureOut = vec3(fragTex, 0.0);
}