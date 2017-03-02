#version 400

in vec2 uv;
uniform sampler2D texture;
uniform float offset;
uniform float lod;

out vec4 color;

void main()
{
    vec4 c = vec4(0);
    c += 5.0 * textureLod(texture, uv - offset, lod);
    c += 6.0 * textureLod(texture, uv, lod);
    c += 5.0 * textureLod(texture, uv + offset, lod);
    color = c / 16.0;
}