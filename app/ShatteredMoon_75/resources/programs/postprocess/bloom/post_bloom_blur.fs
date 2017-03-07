#version 400 core

in vec2 fragTexCoords;
uniform sampler2D glowTexture;
uniform vec2 offset;
uniform float lod;

layout (location = 0) out vec4 blurOut;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    /* Center pixel color */
    vec3 result = texture(glowTexture, fragTexCoords).rgb * weight[0];

    /* Apply blur for surrounding pixels with offset */
    for (int i = 1; i < 5; ++i)
    {
        result += texture(glowTexture, fragTexCoords + offset).rgb * weight[i];
        result += texture(glowTexture, fragTexCoords - offset).rgb * weight[i];
    }

    blurOut = vec4(result, 1.0);
}