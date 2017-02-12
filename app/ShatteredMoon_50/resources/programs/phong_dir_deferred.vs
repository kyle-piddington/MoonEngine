#version 410 core
layout (location = 0) in vec4 position;
layout (location = 2) in vec2 vertTex;
out vec2 fragTex;
void main()
{
	gl_Position = position;
	fragTex = vertTex;
}
