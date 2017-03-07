#version 400 core

in vec2 fragTexCoords;
uniform sampler2D compositeTexture;

layout (location = 0) out vec4 glowOut;

void main()
{
    vec4 color = texture(compositeTexture, fragTexCoords);
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 0.5) {
        glowOut = vec4(color.rgb, 1.0);
    }
    else {
        glowOut = vec4(0.0);
    }
    //glowOut = vec4(color.rgb * 0.09, color.a);
}