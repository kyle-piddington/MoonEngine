#version 410 core
layout (location = 0) in vec4 position;

uniform mat4 P;
uniform mat4 V;

void main()
{
	gl_Position = position;
}
