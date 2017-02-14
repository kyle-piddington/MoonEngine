#version 330 core
in vec3 fragPos;
in vec3 fragNor;
in vec3 fragTex;
out vec4 color;
uniform vec3 tint;
uniform vec3 iGlobalLightDir;
uniform mat4 V;

uniform sampler2D Texture;

void main()
{
	vec3 lightDir = normalize(vec3(V * vec4(iGlobalLightDir,0.0)));
	vec3 ambient = 0.1 * tint;
	vec3 nor=normalize(fragNor);

	vec3 texColor = vec3(texture(Texture, vec2(fragTex)));

	float diff = max(dot(nor,lightDir),0.0f);
	vec3 diffuse = diff * texColor;
	vec3 reflectDir = reflect(-lightDir, nor);
	vec3 viewDir = normalize(vec3(0,0,1) - fragPos);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),8);
	vec3 specular = spec * vec3(1,1,1);
	vec3 result = (diffuse + ambient + specular);
	color = vec4(result,1.0);
}
