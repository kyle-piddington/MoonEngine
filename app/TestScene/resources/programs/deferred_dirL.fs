#version 410

const int NUM_SHADOWS = 3;

struct DirLight
{
    vec3 color;
    vec3 direction;
    float ambient;
};

uniform sampler2D positionTex;
uniform sampler2D colorTex;
uniform sampler2D normalTex;

vec4 LSPosition;
uniform sampler2D shadowMap[NUM_SHADOWS];
uniform mat4 LV[NUM_SHADOWS];
uniform float shadowZSpace[NUM_SHADOWS];
vec4 CascadeIndicator;

uniform DirLight dirLight;

in vec2 fragTexCoord;


float calcShadowFactor(int ShadowIndex, vec4 LSPosition) 
{
    vec3 projCoords = LSPosition.xyz / LSPosition.w;
    projCoords  = 0.5 * projCoords + 0.5;  
    float currentDepth = projCoords.z;     
    float shadowDepth = texture(shadowMap[ShadowIndex], projCoords.xy).r; 
    float bias = 0.0001;
    if (shadowDepth < currentDepth + bias)
        return 0.5;
    else           
        return 1.0;
}  


vec4 calcDirLight(vec3 Diffuse, vec3 Normal, vec3 worldPos, float Specular, float ShadowFactor)
{
    //Ambient
    vec4 AmbientColor = vec4(dirLight.color * dirLight.ambient, 1.0);

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
    float Specular = texture(colorTex, TexCoord).a;
    
    float ShadowFactor = 0.0;
    for (int i = 0 ; i < NUM_SHADOWS ; i++) {
        if (WorldPos.z <= shadowZSpace[i]) {
            LSPosition = LV[i] * vec4(WorldPos, 1.0);
            ShadowFactor = calcShadowFactor(i, LSPosition);

            // if (i == 0) 
            //     CascadeIndicator = vec4(0.1, 0.0, 0.0, 0.0);
            // else if (i == 1)
            //     CascadeIndicator = vec4(0.0, 0.1, 0.0, 0.0);
            // else if (i == 2)
            //     CascadeIndicator = vec4(0.0, 0.0, 0.1, 0.0);
            // else
                CascadeIndicator = vec4(0.0, 0.0, 0.0, 0.0);
        }
   }

	finalColor = calcDirLight(Diffuse, Normal, WorldPos, Specular, ShadowFactor) + CascadeIndicator;
}
