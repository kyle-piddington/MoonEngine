#version 400 core

in vec2 fragTexCoords;
uniform sampler2D _inputTexture;

layout (location = 0) out vec4 blurOut;

void main()
{
    vec4 color = texture(_inputTexture, fragTexCoords);
    blurOut = vec4(color.rgb * 0.09, color.a);
}