#version 330 core
in vec3 fragPos;
in vec3 fragNor;
in vec3 fragTex;

uniform vec3 tint;
uniform sampler2D heightmap;
uniform sampler2D heightmap_normal;

uniform vec2 t_resolution;
uniform vec3 g_cameraPosition;
uniform vec3 iGlobalLightDir;

uniform bool showAO;


layout (location = 0) out vec3 posOut;
layout (location = 1) out vec4 colorOut;
layout (location = 2) out vec3 normalOut;


vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float getAO()
{
	return texture(heightmap_normal,fragTex.xy).r;
}
void main()
{
	//Really dumb AO, lower is darker
	vec3 ambient = vec3(0.05) *texture(heightmap,fragTex.xy).r ;
	float AO = getAO();
	
	vec3 diffuse = vec3(0.5, 0.2, 0.06);
	//float spec = pow(max(dot(reflectDir,viewDir),0.0),32);
	//vec3 specular = spec * vec3(1,1,1);
	
	vec3 result = (diffuse + ambient);
	if(showAO)
	{
		result *= min(1, (0.5 + AO));
	}
	
	posOut = fragPos;
	colorOut = vec4(result,0.1);
	normalOut = fragNor;

}
