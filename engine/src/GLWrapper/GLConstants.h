#pragma once
#define GL_VERTEX_POSITION_ATTRIBUTE 0
#define GL_VERTEX_NORMAL_ATTRIBUTE 1
#define GL_VERTEX_TEXTURE_ATTRIBUTE 2
//Matrix instance binding takes up four attributes.
#define GL_VERTEX_INSTANCE_MATRIX_ATTRIBUTE 3
#define GL_VERTEX_TANGENT_ATTRIBUTE 7
#define GL_VERTEX_BITANGENT_ATTRIBUTE 8
//Bone mapping uniforms
#define GL_VERTEX_BONE_ID_ATTRIBUTE 9
#define GL_VERTEX_BONE_WEIGHT_ATTRIBUTE 10


#define GL_WRITE_BUFFER 0x0C03
#define GL_READ_WRITE_BUFFER 0x0C04

#define POSITION_TEXTURE "_gBufferPosition"
#define NORMAL_TEXTURE "_gBufferNormal"
#define DEPTH_STENCIL_TEXTURE "_gBufferDepth"
#define COLOR_TEXTURE "_gBufferColor"
#define COMPOSITE_TEXTURE "_gBufferCompositeTexture"

#define SSAO_COLOR_TEXTURE "_gSSAOColorTex"
#define SSAO_BLUR_TEXTURE "_gSSAOBlurTex"
#define SSAO_NOISE_TEXTURE "_gSSAONoise"

#define BLOOM_GLOW_TEXTURE "_gBloomGlow"
#define BLOOM_TEMP_TEXTURE "_gBloomTemp"

#define SHADOW_TEXTURE "_shadowLevel"

#define POSITION_ATTACHMENT GL_COLOR_ATTACHMENT0
#define COLOR_ATTACHMENT GL_COLOR_ATTACHMENT1
#define NORMAL_ATTACHMENT GL_COLOR_ATTACHMENT2
#define COMPOSITE_ATTACHMENT GL_COLOR_ATTACHMENT4
#define SSAO_ATTACHMENT GL_COLOR_ATTACHMENT0
#define SSAO_NOISE_ATTACHMENT GL_COLOR_ATTACHMENT5

