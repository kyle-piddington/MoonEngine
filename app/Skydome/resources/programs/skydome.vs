#version 400 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 fragPos;

void main()
{
	gl_Position = P * V * M * position;
	fragPos = vec3(V * M * position);
}