#version 410

struct DirLight
{
    vec3 color;
    vec3 direction;
    float ambient;
};

uniform sampler2D positionTex;
uniform sampler2D colorTex;
uniform sampler2D normalTex;
uniform sampler2D ssaoTex;

uniform DirLight dirLight;

in vec2 fragTexCoord;

vec4 calcDirLight(vec3 Diffuse, float ssao, vec3 Normal, vec3 worldPos, float Specular, float ShadowFactor)
{
    //Ambient
    vec4 AmbientColor = vec4(dirLight.color * dirLight.ambient * ssao, 1.0);

    // Diffuse
    vec3 lightDir = normalize(-dirLight.direction);
    vec4 DiffuseColor = vec4(max(dot(Normal, lightDir), 0.0) * Diffuse * dirLight.color, 1.0);

    // Specular
    vec3 cameraDir = normalize(-worldPos);
    vec3 halfDir = normalize(lightDir + cameraDir);  
    float specPercent = pow(max(dot(Normal, halfDir), 0.0), 16.0);
    vec4 SpecularColor = vec4(dirLight.color * specPercent * Specular, 1.0);
    
    return (AmbientColor + ShadowFactor* (DiffuseColor + SpecularColor));
}


vec2 locTexCoord()
{
    return fragTexCoord;
}

out vec4 finalColor;

void main()
{
    vec2 TexCoord = locTexCoord();
	vec3 WorldPos = texture(positionTex, TexCoord).xyz;
	vec3 Diffuse = texture(colorTex, TexCoord).rgb;
	vec3 Normal = texture(normalTex, TexCoord).xyz;
    Normal = normalize(Normal);
    float ssao = texture(ssaoTex, TexCoord).r;
    float Specular = texture(colorTex, TexCoord).a;
    float ShadowFactor = texture(normalTex, TexCoord).a;
    finalColor = vec4(0);
    //finalColor = vec4(ssao);
    finalColor = calcDirLight(Diffuse, ssao, Normal, WorldPos, Specular, ShadowFactor);
}
