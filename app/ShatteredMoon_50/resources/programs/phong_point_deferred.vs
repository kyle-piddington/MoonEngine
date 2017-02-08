#version 400 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texPos;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 fragTex;

void main()
{
	gl_Position = P * V * M *  position;
	fragTex = texPos;
}
