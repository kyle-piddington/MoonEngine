#version 400 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in mat4 I;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat3 N;
uniform float windTime;
uniform float windSpeed;
uniform float windStrength;
uniform float windOsc;
uniform vec2 windDir = vec2(1,0.7);
uniform vec3 player;
out vec2 UV;
out vec3 fragNor;
out vec3 fragPos;

mat4 calcShear(float shearStr, vec2 shearDir)
{
	mat4 shear = mat4(1,0,0,0,
					  shearDir.x * shearStr, 1, shearDir.y * shearStr,0,
					  0,0,1,0,
					  0,0,0,1);
	return shear;
}

void main()
{
	vec2 wDir = normalize(windDir);
	vec3 centerPoint = vec3(I[3][0],I[3][1],I[3][2]);

	mat4 anim = calcShear(windStrength*sin(windSpeed*windTime + windOsc * dot(wDir, centerPoint.xz)),wDir);
	vec3 playerPushPos = normalize(centerPoint - player);
	mat4 playerShearAnim = 
		calcShear(
			0.25*max(0,2 - length(centerPoint - player))
			,playerPushPos.xz);

	mat3 N = transpose(inverse(V*I*anim*playerShearAnim));

	fragPos = V *  I * anim * playerShearAnim * position;
	fragNor = N * vec3(0,1,0);
	gl_Position = P *fragPos;
	UV = texCoords;
}
