#version 400 core
in vec3 fragPos;
in vec3 fragNor;
in vec2 fragTex;

const int NUM_SHADOWS = 3;

in vec4 LSPosition[NUM_SHADOWS];
in float worldZ;

uniform sampler2D shadowMap[NUM_SHADOWS];
uniform float shadowZSpace[NUM_SHADOWS];

layout (location = 0) out vec4 posOut;
layout (location = 1) out vec4 colorOut;
layout (location = 2) out vec4 normalOut;

uniform sampler2D diffuse;


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

void main()
{
    posOut = vec4(fragPos, 1.0);
    colorOut.rgb = texture(diffuse, fragTex).rgb;
    colorOut.a = 1; //Currently hardcoded specular
    normalOut.xyz = normalize(fragNor);

    float ShadowFactor = 0.0;
    for (int i = 0 ; i < NUM_SHADOWS ; i++) {
        if (worldZ >= shadowZSpace[i]) {
            ShadowFactor = calcShadowFactor(i, LSPosition[i]);
            normalOut.a = ShadowFactor;
            break;
        }
   }

}