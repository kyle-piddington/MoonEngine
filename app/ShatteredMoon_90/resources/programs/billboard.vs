#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertTexCoords;

out vec2 UV;
out vec3 sunPos;

uniform mat4 V;
uniform mat4 P;
uniform mat4 M;

uniform vec3 dirLight;

void main()
{

   	mat4 MV = V * M;
   	// Column 0:
	MV[0][0] = M[0][0];
	MV[0][1] = 0;
	MV[0][2] = 0;

	// Column 2:
	MV[2][0] = 0;
	MV[2][1] = 0;
	MV[2][2] = M[2][2];

	gl_Position = P * MV * vec4(position, 1.0f);
	UV = vertTexCoords;
}