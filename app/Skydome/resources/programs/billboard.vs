#version 400 core

layout(location = 0) in vec3 position;

out vec2 UV;

uniform mat4 V;
uniform mat4 P;

uniform vec3 iGlobalLightDir;

//uniform vec3 BillboardCenter;
//uniform vec2 BillboardSize;

void main()
{

    vec3 CameraRight = vec3(V[0][0], V[1][0], V[2][0]);
    vec3 CameraUp = vec3(V[0][1], V[1][1], V[2][1]);

    vec3 pos = vec3(iGlobalLightDir.x, iGlobalLightDir.y + 150.0, iGlobalLightDir.z);
	vec3 vertexPosition = pos + CameraRight * position.x + CameraUp * position.y;
		/*BillboardCenter
		+ CameraRight * position.x * BillboardSize.x
		+ CameraUp * position.y * BillboardSize.y;*/

	gl_Position = P * V * vec4(vertexPosition, 1.0f);

	UV = position.xy + vec2(0.5, 0.5);
}
