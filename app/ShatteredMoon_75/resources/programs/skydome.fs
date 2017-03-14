#version 400 core

in vec3 fragPos;

uniform sampler2D skycolor;
uniform float iGlobalTime;

out vec3 color;

void main()
{
    vec3 normalPos = normalize(fragPos);

    float currentTime = (iGlobalTime + 1.0) / 2.0;
    float horizon = max(normalPos.y, 0.01);
    vec2 texturePos = vec2(currentTime, horizon);

    color = texture(skycolor, texturePos).rgb;
}
