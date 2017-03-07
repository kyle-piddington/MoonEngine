#version 400 core

layout(location = 0) in vec3 position;

out vec2 UV;
out vec3 sunPos;

uniform mat4 V;
uniform mat4 P;
uniform mat4 M;

uniform vec3 dirLight;

void main()
{

    // Commented code if size should scale on distance */
    //vec3 CameraRight = vec3(V[0][0], V[1][0], V[2][0]);
    //vec3 CameraUp = vec3(V[0][1], V[1][1], V[2][1]);

    //vec2 scale = vec2(M[0][0], M[1][1]);

    vec3 pos = vec3(dirLight.x, dirLight.y + 200.0, dirLight.z);
	/*pos = pos + CameraRight * position.x * scale.x +
	    CameraUp * position.y * scale.y;*/

	gl_Position = P * V * vec4(pos, 1.0f);
	/* Reset perspective */
	gl_Position /= gl_Position.w;
	/* Scale to percent of screen */
	gl_Position.xy += position.xy * vec2(0.15, 0.2);

	UV = position.xy + vec2(0.5, 0.5);
	sunPos = normalize(M * vec4(-dirLight, 1.0)).xyz;
}
