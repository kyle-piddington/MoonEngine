#version 330 core
in vec3 fragPos;
in vec3 fragTex;
out vec4 color;
uniform vec3 tint;
uniform sampler2D heightmap;
uniform sampler2D heightmap_normal;

uniform vec2 t_resolution;
uniform vec3 g_cameraPosition;
uniform vec3 iGlobalLightDir;


vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4 getNormal()
{
	return texture(heightmap_normal,fragTex.xy);
}
void main()
{
	vec3 lightDir = normalize(iGlobalLightDir);
	//Really dumb AO, lower is darker
	vec3 ambient = vec3(0.05) *texture(heightmap,fragTex.xy).r ;
	vec3 viewDir = normalize(g_cameraPosition - fragPos);
	vec4 norAO = getNormal();
	
	float diff = max(dot(norAO.xyz * 2 - 1,lightDir),0.0f);
	vec3 diffuse = vec3(0.8,0.3,0.3) * diff;
	//float spec = pow(max(dot(reflectDir,viewDir),0.0),32);
	//vec3 specular = spec * vec3(1,1,1);
	vec3 result = (diffuse + ambient);
	
	color = vec4(result * norAO.w,1.0);
}
