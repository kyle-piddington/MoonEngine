#version 400 core

in vec2 fragTexCoords;
uniform sampler2D compositeTexture;
uniform sampler2D depthTexture;

layout (location = 0) out vec4 colorOut;

void main()
{
    // Fog parameters, could make them uniforms and pass them into the fragment shader
    float fog_maxdist = 8.0;
    float fog_mindist = 0.1;
    vec4  fog_colour = vec4(0.4, 0.4, 0.4, 1.0);

    // Calculate fog
    vec4 depth = texture(depthTexture, fragTexCoords);
    float dist = length(depth);
    float fog_factor = (fog_maxdist - dist) /
                      (fog_maxdist - fog_mindist);
    fog_factor = clamp(fog_factor, 0.0, 1.0);

    vec4 color = texture(compositeTexture, fragTexCoords);
    colorOut = mix(fog_colour, color, fog_factor);
}