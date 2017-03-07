#version 400 core

in vec3 fragPos;

uniform sampler2D skycolor;
uniform float iGlobalTime;

out vec3 color;

void main()
{
    vec3 normalPos = normalize(fragPos);

    float horizon = max(normalPos.y, 0.01);
    vec2 texturePos = vec2(iGlobalTime, horizon);

    color = texture(skycolor, texturePos).rgb;
}
