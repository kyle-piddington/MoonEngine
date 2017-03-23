#version 400 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture;
layout (location = 9)  in vec4 boneIDs;
layout (location = 10)  in vec4 boneWeights;

const int NUM_SHADOWS = 3;
const int MAX_BONES = 100;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform mat4 gBinds[MAX_BONES];
uniform mat4 gBones[MAX_BONES];

void main()
{
	uvec4 intIdx = uvec4(floor(boneIDs));//boneIDs.x, boneIDs.y, boneIDs.z;
	mat4 boneTransform = gBones[intIdx[0]] * gBinds[intIdx[0]] * boneWeights[0];
    boneTransform +=     gBones[intIdx[1]] * gBinds[intIdx[1]] * boneWeights[1];
    boneTransform +=     gBones[intIdx[2]] * gBinds[intIdx[2]] * boneWeights[2];
    boneTransform +=     gBones[intIdx[3]] * gBinds[intIdx[3]] * boneWeights[3];
    
    vec4 worldPosition = (M * boneTransform * position);
	vec4 fragPos4 = V *worldPosition;
	gl_Position = P * fragPos4;
	
}