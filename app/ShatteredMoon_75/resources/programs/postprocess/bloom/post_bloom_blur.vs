#version 400
layout (location = 2) in vec2 texture;

out vec2 uv;
void main()
{
	uv = texture;
}