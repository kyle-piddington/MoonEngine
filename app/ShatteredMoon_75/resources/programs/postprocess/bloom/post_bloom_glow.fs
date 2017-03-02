#version 400 core

in vec2 fragTexCoords;
uniform sampler2D _inputTexture;

layout (location = 0) out vec4 blurOut;

void main()
{
    vec4 color = texture(_inputTexture, fragTexCoords);
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 0.5) {
        blurOut = vec4(color.rgb, 1.0);
    }
    else {
        blurOut = vec4(0.0);
    }
}