#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat3 N;
out vec3 fragPos;
out vec3 fragNor;
void main()
{
	gl_Position = P * V * M *  position;
	fragPos = vec3(V * M * position);
	fragNor = N * normal;
}
