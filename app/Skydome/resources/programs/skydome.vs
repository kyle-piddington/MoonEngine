#version 400 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 iGlobalLightDir;

out vec3 fragPos;
out vec3 sunPos;

void main()
{
	gl_Position = P * V * M * position;
	fragPos = vec3(M * position);

	sunPos = normalize(M * vec4(iGlobalLightDir, 1.0)).xyz;
}