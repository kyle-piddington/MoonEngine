#version 400 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture;
layout (location = 9)  in ivec4 boneIDs;
layout (location = 10)  in ivec4 boneWeights;

const int NUM_SHADOWS = 3;
const int MAX_BONES 100;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat3 N;

uniform mat4 gBinds[MAX_BONES];
uniform mat4 gBones[MAX_BONES];

uniform mat4 LV[NUM_SHADOWS];
out vec3 fragPos;
out vec3 fragNor;
out vec2 fragTex;
out vec4 LSPosition[NUM_SHADOWS];
out float worldZ;

void main()
{
	mat4 BoneTransform = gBones[BoneIDs[0]] * gBinds[BoneIDs[0]] * Weights[0];
    BoneTransform +=     gBones[BoneIDs[1]] * gBinds[BoneIDs[1]] * Weights[1];
    BoneTransform +=     gBones[BoneIDs[2]] * gBinds[BoneIDs[2]] * Weights[2];
    BoneTransform +=     gBones[BoneIDs[3]] * gBinds[BoneIDs[3]] * Weights[3];
    
    vec3 worldPosition = M * boneTransform * position;

	fragPos = V *worldPosition;
	gl_Position = P * fragPos;
	for (int i = 0 ; i < NUM_SHADOWS ; i++) {
        LSPosition[i] = LV[i] * worldPosition
    }
    worldZ = gl_Position.z;
	
	fragNor = mat3(inverse(boneTransform)) * N * normal;
	fragTex = texture;
}