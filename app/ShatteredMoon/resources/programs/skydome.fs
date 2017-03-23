#version 400 core

in vec3 fragPos;

uniform sampler2D skycolor;
uniform float iGlobalTime;

out vec3 color;

void main()
{
    vec3 normalPos = normalize(fragPos);

    float currentTime = iGlobalTime;
    float horizon = max(normalPos.y, 0.01);
    vec2 texturePos = vec2(currentTime, horizon);

    color = texture(skycolor, texturePos).rgb;
    color *= 1.6 - currentTime / 2.0;
}
