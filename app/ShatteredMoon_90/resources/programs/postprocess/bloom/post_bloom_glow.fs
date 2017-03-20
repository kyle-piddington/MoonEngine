#version 400 core

in vec2 fragTexCoords;
uniform sampler2D compositeTexture;

layout (location = 0) out vec4 glowOut;

void main()
{
    vec4 color = texture(compositeTexture, fragTexCoords);
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));

    if (brightness > 0.6) {
        glowOut = vec4(color.rgb, 1.0);
    }
    else if (brightness > 0.3) {
        float edge = smoothstep(0.3, 0.6, brightness);

        glowOut = vec4(mix(vec3(0.0), color.rgb, edge), 1.0);
    }
    else {
        glowOut = vec4(vec3(0.0), 1.0);
    }

    //glowOut = vec4(color.rgb * 0.09, color.a);
}