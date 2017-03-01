#version 400 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture;

const int NUM_SHADOWS = 3;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat3 N;

uniform mat4 LV[NUM_SHADOWS];

out vec3 fragPos;
out vec3 fragNor;
out vec2 fragTex;
out vec4 LSPosition[NUM_SHADOWS];
out float worldZ;

void main()
{
	gl_Position = P * V * M * position;

	for (int i = 0 ; i < NUM_SHADOWS ; i++) {
        LSPosition[i] = LV[i] * position;
    }
    worldZ = gl_Position.z;
	fragPos = vec3(V * M * position);
	fragNor = N * normal;
	fragTex = texture;
}