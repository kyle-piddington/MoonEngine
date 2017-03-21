#version 400 core
layout (location = 0) in vec4 position;
layout (location = 2) in vec2 texture;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 fragTexCoords;
void main()
{
	gl_Position = P * M * position;
	fragTexCoords = texture;
}
