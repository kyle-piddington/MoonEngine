#version 400

in vec2 uv;
uniform sampler2D texture;
uniform float offset;
uniform int lod;

layout (location = 0) out vec4 blurOut;

void main()
{
    vec4 c = vec4(0);
    c += 5.0 * textureLod(texture, uv - offset, lod);
    c += 6.0 * textureLod(texture, uv, lod);
    c += 5.0 * textureLod(texture, uv + offset, lod);
    blurOut = c / 16.0;
}