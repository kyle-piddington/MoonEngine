#version 330 core
/**
 * Core logic credit to Filip Strugar. (2009)
 * CDLOD Implementation  of vertex shaders.
 */
layout (location = 0) in vec4 position;

struct terrainInput
{
	vec4 g_quadOffset;
	vec4 g_quadScale;
	vec4 g_morphConsts;
};
	
uniform terrainInput t_param;

uniform mat4 V;
uniform mat4 P;
uniform mat3 N;

uniform vec3 g_cameraPosition;
uniform vec2 g_samplerWorldToTextureScale;
uniform vec3 g_gridDim;
uniform vec4 g_heightmapTextureInfo; // .xy = texturesize .zw = texelsize.
uniform vec4 g_terrainScale;
uniform vec4 g_terrainOffset;
uniform vec2 g_quadWorldMax;

out vec3 fragPos;
out vec3 fragWorldPos;
out vec3 fragWorldNor;
out vec3 fragNor;
out vec3 fragTex;
uniform sampler2D heightmap;
uniform sampler2D heightmap_normal;


vec4 getBaseVertexPos( vec4 inPos )
{
   vec4 ret = inPos * t_param.g_quadScale + t_param.g_quadOffset;
   ret.xz = min( ret.xz, g_quadWorldMax );
   return ret;
}

vec2 morphVertex( vec4 inPos, vec2 vertex, float morphLerpK )
{
   vec2 fracPart = (fract(inPos.xz * vec2(g_gridDim.y, g_gridDim.y) ) 
   					* vec2(g_gridDim.z, g_gridDim.z) ) * t_param.g_quadScale.xz;
   return vertex.xy - fracPart * morphLerpK;
}

vec2 calcGlobalUV( vec2 vertex )
{
   vec2 globalUV = (vertex.xy - g_terrainOffset.xz) / g_terrainScale.xz;  // this can be combined into one inPos * a + b
   globalUV *= g_samplerWorldToTextureScale;
   globalUV += g_heightmapTextureInfo.zw * 0.5;
   return globalUV;
}

float sampleHeightmap(vec2 globalUV)
{
	return texture(heightmap, globalUV).r;
}

void processCDLODVertex(
	vec4 inPos, 
	out vec4 outUnmorphedWorldPos, 
	out vec4 outWorldPos, 
	out vec2 outGlobalUV,
	out float outMorphK, 
	out float outEyeDist )
{
	vec4 vertex = getBaseVertexPos(inPos);
	float lodLevel = t_param.g_quadScale.z;

	//Sample heightmap for morphing value later.
	float mipLevel = 0;
	vec2 preGlobalUV = calcGlobalUV(vertex.xz);
	vertex.y  = sampleHeightmap(preGlobalUV) * g_terrainScale.y + g_terrainOffset.y;

	outUnmorphedWorldPos = vertex;
	outUnmorphedWorldPos.w = 1;

	float eyeDist = distance(vertex.xyz, g_cameraPosition);
	//Morph based off of distance from camera.
	float morphLerpK  = 1.0f - clamp( t_param.g_morphConsts.z - eyeDist * t_param.g_morphConsts.w, 0.0, 1.0 );   
	vertex.xz  = morphVertex(inPos, vertex.xz, morphLerpK);
	vec2 globalUV = calcGlobalUV(vertex.xz);

	vertex.y = sampleHeightmap(globalUV) * g_terrainScale.y + g_terrainOffset.y;
	//Resample heightmap with new vertex
	vertex.w = 1.0;
	outWorldPos = vertex;
	outGlobalUV = globalUV;
	outMorphK = morphLerpK;
	outEyeDist = eyeDist;
}


vec3 getNormal(vec2 globalUV)
{
	return texture(heightmap_normal,globalUV).xyz;
}
void main()
{
	vec4 outUnmorphedWorldPos;
	vec4 worldPos;
	vec2 globalUV;
	float morphK;
	float eyeDist;
	processCDLODVertex(position, outUnmorphedWorldPos, worldPos, globalUV, morphK, eyeDist);
	vec4 camVert = V * worldPos;
	fragWorldPos = worldPos.xyz;
	fragPos = camVert.xyz;
	fragWorldNor = getNormal (globalUV); 
	fragNor = N * fragWorldNor;
	fragTex.xy = globalUV.xy;
	fragTex.z = morphK;
	gl_Position = P * camVert;

}


