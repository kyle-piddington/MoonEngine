#version 400 core

in vec2 fragTexCoords;
uniform sampler2D glowTexture;
uniform vec2 offset;
uniform int lod;

layout (location = 0) out vec4 blurOut;

void main()
{
    vec4 c = vec4(0);
    c += 5.0 * textureLod(glowTexture, fragTexCoords - offset, lod);
    c += 6.0 * textureLod(glowTexture, fragTexCoords, lod);
    c += 5.0 * textureLod(glowTexture, fragTexCoords + offset, lod);
    blurOut = c / 16.0;
}