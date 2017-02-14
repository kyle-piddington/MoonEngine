#version 410

struct Attenuation
{
    float constant;
    float linear;
    float exp;
};

struct PointLight
{
    vec3 color;
    vec3 position;
    float ambient;
    Attenuation atten;
};

uniform sampler2D positionTex;
uniform sampler2D colorTex;
uniform sampler2D normalTex;
uniform PointLight pointLight;

uniform vec3 cameraPosition;
uniform vec2 screenSize;

vec4 calcLightEffect(vec3 WorldPos, vec3 Diffuse, vec3 Normal, float Specular)
{
    //Ambient
    vec4 AmbientColor = vec4(pointLight.color * pointLight.ambient, 1.0);

    // Diffuse
    vec3 lightDir = normalize(pointLight.position - WorldPos);
    vec4 DiffuseColor = vec4(max(dot(Normal, lightDir), 0.0) * Diffuse * pointLight.color, 1.0);

    // Specular
    vec3 halfDir = normalize(lightDir + cameraPosition);  
    float specPercent = pow(max(dot(Normal, halfDir), 0.0), 16.0);
    vec4 SpecularColor = vec4(pointLight.color * specPercent * Specular, 1.0);
    
    return (AmbientColor + DiffuseColor + SpecularColor);
}


vec4 calcPointLight(vec3 WorldPos, vec3 Diffuse, vec3 Normal, float Specular)
{
    float distance = length(WorldPos - pointLight.position);

    vec4 baseColor = calcLightEffect(WorldPos, Diffuse, Normal, Specular);
    
    float attenTotal  =  pointLight.atten.constant +
                         pointLight.atten.linear * distance +
                         pointLight.atten.exp * distance * distance;

    attenTotal = max(1.0, attenTotal);

    return baseColor / attenTotal;
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

    finalColor = calcPointLight(WorldPos, Diffuse, Normal, Specular);
}
