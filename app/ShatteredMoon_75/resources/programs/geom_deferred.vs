#version 400 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat3 N;

out vec3 fragPos;
out vec3 fragNor;
out vec2 fragTex;

void main()
{
	gl_Position = P * V * M * position;
	fragPos = vec3(V * M * position);
	fragNor = N * normal;
	fragTex = texture;
}