#version 400 core
in vec2 fragTex;

out vec4 color;

uniform vec3 tint;
uniform vec3 iGlobalLightDir;

uniform sampler2D positionIn;
uniform sampler2D diffuseIn;
uniform sampler2D textureIn;
uniform sampler2D normalIn;

uniform mat4 V;

void main()
{
    //TODO multiple lights
    // https://learnopengl.com/code_viewer.php?code=advanced-lighting/deferred&type=fragment
	vec3 position = texture(positionIn, fragTex).rgb;
	vec3 diffuse = texture(diffuseIn, fragTex).rgb;
	vec3 normal = texture(normalIn, fragTex).rgb;

	vec3 lightDir = normalize(vec3(V * vec4(iGlobalLightDir, 0.0)));
	vec3 ambient = 0.3 * tint;
	vec3 nor = normalize(normal);

	vec3 texColor = vec3(texture(diffuse, vec2(fragTex)));

	float diff = max(dot(nor, lightDir), 0.0f);
	vec3 diffuse_color = diff * diffuse * tint;
	vec3 reflectDir = reflect(-lightDir, nor);
	vec3 viewDir = normalize(vec3(0,0,1) - position);
	float spec = pow(max(dot(reflectDir, viewDir),0.0),32);
	vec3 specular = spec * vec3(1,1,1);
	vec3 result = (diffuse_color + ambient + specular);
	color = vec4(result,1.0);
}
