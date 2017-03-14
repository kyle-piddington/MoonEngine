#version 410 core
layout (location = 0) in vec4 position;
layout (location = 2) in vec2 textureCoordinates;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
	gl_Position = P * V * M *  position;

}
