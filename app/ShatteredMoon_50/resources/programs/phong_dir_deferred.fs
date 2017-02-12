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
uniform DirLight dirLight;

uniform vec3 cameraPosition;
uniform vec2 screenSize;

vec4 calcDirLight(vec3 Diffuse, vec3 Normal, float Specular)
{
    //Ambient
    vec4 AmbientColor = vec4(dirLight.color * dirLight.ambient, 1.0);

    // Diffuse
    vec3 lightDir = dirLight.direction;
    vec4 DiffuseColor = vec4(max(dot(Normal, lightDir), 0.0) * Diffuse * dirLight.color, 1.0);

    // Specular
    vec3 halfDir = normalize(lightDir + cameraPosition);  
    float specPercent = pow(max(dot(Normal, halfDir), 0.0), 16.0);
    vec4 SpecularColor = vec4(dirLight.color * specPercent * Specular, 1.0);
    
    return (AmbientColor + DiffuseColor + SpecularColor);
}

vec2 locTexCoord()
{
    return gl_FragCoord.xy / screenSize;
}

out vec4 finalColor;

void main()
{
    vec2 TexCoord = locTexCoord();
	vec3 WorldPos = texture(positionTex, TexCoord).xyz;
	vec3 Diffuse = texture(colorTex, TexCoord).rgb;
	vec3 Normal = texture(normalTex, TexCoord).xyz;
    Normal = normalize(Normal);
    float Specular = texture(colorTex, TexCoord).a;
    

	finalColor = calcDirLight(Diffuse, Normal, Specular);
}
