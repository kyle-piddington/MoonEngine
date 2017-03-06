#version 400
layout (location = 0) in vec4 position;
layout (location = 2) in vec2 texture;

out vec2 fragTexCoords;

void main()
{
	gl_Position = position;
	fragTexCoords = texture;
}