#version 400

in vec2 UV;
in vec3 fragPos;
in vec3 fragNor;
out vec4 color;
uniform sampler2D diffuse;

layout (location = 0) out vec4 posOut;
layout (location = 1) out vec4 colorOut;
layout (location = 2) out vec4 normalOut;

void main()
{
    float alphaThresh = texture(diffuse, UV).a;
    if (alphaThresh < 0.2) {
        discard;
    }
    colorOut = mix(vec4(0.5,0.6,0.0,1.0), vec4(0.3,0.2,0.2,1.0), 1-UV.x);
    normalOut = fragNor;
    posOut = fragPos;
}