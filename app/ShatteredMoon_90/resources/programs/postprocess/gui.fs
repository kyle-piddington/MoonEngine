#version 400 core
in vec2 fragTexCoords;
uniform sampler2D _guiTexture;
out vec4 color;

void main()
{
	vec4 guiColor = texture(_guiTexture, fragTexCoords);
	// Hacky fix for text not being fully transparent
	if (guiColor.a < 0.3 && guiColor.r > 0.1) {
	    discard;
	}
	color = guiColor;
}