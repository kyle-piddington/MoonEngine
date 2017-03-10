#version 400 core
in vec2 fragTexCoords;
uniform sampler2D _guiTexture;
out vec4 color;

void main()
{
	color = texture(_guiTexture, fragTexCoords);
}