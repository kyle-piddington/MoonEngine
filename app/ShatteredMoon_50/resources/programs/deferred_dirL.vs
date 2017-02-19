#version 410 core
layout (location = 0) in vec4 position;
layout (location = 2) in vec2 texture;

uniform mat4 P;
uniform mat4 V;
out vec2 fragTexCoord;

void main()
{
	gl_Position = position;
	fragTexCoord = texture;
}
