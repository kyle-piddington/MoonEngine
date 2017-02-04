#version 400 core
in vec3 fragPos;
in vec3 fragNor;
in vec2 fragTex;
out vec4 color;
uniform vec3 tint;
uniform vec3 lightDirWorld;
uniform mat4 V;

uniform sampler2D diffuse;

void main()
{
	vec3 lightDir = normalize(vec3(V * vec4(lightDirWorld,0.0)));
	vec3 ambient = 0.3 * tint;

	vec3 texColor = vec3(texture(diffuse, vec2(fragTex)));

	vec3 nor=normalize(fragNor);
	float diff = max(dot(nor,lightDir),0.0f);
	vec3 diffuse_color = diff * tint * texColor;
	vec3 reflectDir = reflect(-lightDir, nor);
	vec3 viewDir = normalize(vec3(0,0,1) - fragPos);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),32);
	vec3 specular = spec * vec3(1,1,1);
	vec3 result = (diffuse_color + ambient + specular);
	color = vec4(result,1.0);
}
