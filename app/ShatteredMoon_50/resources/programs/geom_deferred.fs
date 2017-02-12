#version 400 core
in vec3 fragPos;
in vec3 fragNor;
in vec2 fragTex;

layout (location = 0) out vec3 posOut;
layout (location = 1) out vec4 colorOut;
layout (location = 2) out vec3 normalOut;

uniform sampler2D diffuse;

void main()
{
    posOut = fragPos;
    colorOut.rgb = texture(diffuse, fragTex).rgb;
    colorOut.a = 1; //Currently hardcoded specular
    normalOut = normalize(fragNor);
}