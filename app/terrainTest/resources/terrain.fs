#version 330 core
in vec3 fragPos;
in vec3 fragTex;
out vec4 color;
uniform vec3 tint;
uniform sampler2D heightmap;
uniform vec2 t_resolution;
uniform vec3 g_cameraPosition;
uniform vec3 iGlobalLightDir;


vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 getNormal()
{

	const float strength = 2;
	vec3 offset = vec3(-1.0/t_resolution.x, 0.0, 1.0/t_resolution.x);
	float s1 = texture(heightmap,fragTex.xy + offset.xy).r;
	float s2 = texture(heightmap,fragTex.xy + offset.zy).r;
	float s3 = texture(heightmap,fragTex.xy + offset.yx).r;
	float s4 = texture(heightmap,fragTex.xy + offset.yz).r;
	vec3 va = vec3(1.0, strength * (s2 - s1), 0.0);
	vec3 vb = vec3(0.0,  strength * (s4 - s3), 1.0);
	vec3 n = normalize(cross(va, vb));
	return n;
}
void main()
{
	vec3 lightDir = iGlobalLightDir;
	vec3 ambient = vec3(0.3) *texture(heightmap,fragTex.xy).r ;
	vec3 viewDir = normalize(g_cameraPosition - fragPos);
	vec3 nor = -getNormal();
	
	float diff = max(dot(nor,lightDir),0.0f);
	vec3 diffuse = vec3(0.8,0.6,0.6) * diff;
	//float spec = pow(max(dot(reflectDir,viewDir),0.0),32);
	//vec3 specular = spec * vec3(1,1,1);
	vec3 result = (diffuse + ambient);
	
	color = vec4(result ,1.0);
}
