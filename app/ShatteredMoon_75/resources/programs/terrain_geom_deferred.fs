#version 330 core
in vec3 fragWorldPos;
in vec3 fragWorldNor;
in vec3 fragPos;
in vec3 fragNor;
in vec3 fragTex;

uniform vec3 tint;
uniform sampler2D heightmap;
uniform sampler2D heightmap_normal;
uniform sampler2D diffuse;
uniform sampler2D canyonTint;

//Shadow uniforms
const int NUM_SHADOWS = 3;
uniform sampler2D shadowMap[NUM_SHADOWS];
uniform float shadowZSpace[NUM_SHADOWS];
in vec4 LSPosition[NUM_SHADOWS];
in float worldZ;


uniform vec2 t_resolution;
uniform vec3 g_cameraPosition;
uniform vec3 iGlobalLightDir;

uniform bool showAO;


layout (location = 0) out vec4 posOut;
layout (location = 1) out vec4 colorOut;
layout (location = 2) out vec4 normalOut;


vec3 color[3] = vec3[](vec3(0.1,0,0), vec3(0,0.1,0), vec3(0,0,0.1));

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

float getAO()
{
	return texture(heightmap_normal,fragTex.xy).r;
}

//https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float rand(float n){return fract(sin(n) * 43758.5453123);}

float rand(vec2 n) { 
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}


float noise(vec2 n) {
    const vec2 d = vec2(0.0, 1.0);
  vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
    return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}

void main()
{
	//Really dumb AO, lower is darker
	vec3 ambient = vec3(0.05) *texture(heightmap,fragTex.xy).r ;
	float AO = getAO();
	
	vec3 diffColor = vec3(0.5, 0.2, 0.06);
	//float spec = pow(max(dot(reflectDir,viewDir),0.0),32);
	//vec3 specular = spec * vec3(1,1,1);
	
	vec3 result = (diffColor + ambient);
	if(showAO)
	{
		result *= min(1, (0.5 + AO));
	}

	//Texture lookups:
	//https://gamedevelopment.tutsplus.com/articles/use-tri-planar-texture-mapping-for-better-terrain--gamedev-13821
	vec3 blend = abs(normalize(fragWorldNor));
	blend = normalize(max(blend, 0.00001));
	float b = blend.x + blend.y + blend.z;
	blend /= vec3(b);
	vec4 xaxis = texture( diffuse, fragWorldPos.yz);
	vec4 yaxis = texture( diffuse, fragWorldPos.xz);
	vec4 zaxis = texture( diffuse, fragWorldPos.xy);
	// blend the results of the 3 planar projections.
	vec4 tex = xaxis * blend.x + yaxis * blend.y + zaxis * blend.z;
	tex *= texture(canyonTint, vec2(0,(fragWorldPos.y + 0.5*noise(fragWorldPos.xy))/200));
	
	posOut = vec4(fragPos,1);
	colorOut = vec4( result * tex.xyz,0.1);
	normalOut.rgb = fragNor;
	normalOut.a = 1;
	float ShadowFactor = 0.0;
    for (int i = 0 ; i < NUM_SHADOWS ; i++) {
        if (worldZ <= -shadowZSpace[i]) {
            ShadowFactor = calcShadowFactor(i, LSPosition[i]);
            //colorOut.rgb += color[i];
            normalOut.a = ShadowFactor;
            break;
            
        }
    }

}
