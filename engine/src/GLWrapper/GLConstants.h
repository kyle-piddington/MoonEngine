#pragma once
#define GL_VERTEX_POSITION_ATTRIBUTE 0
#define GL_VERTEX_NORMAL_ATTRIBUTE 1
#define GL_VERTEX_TEXTURE_ATTRIBUTE 2
//Matrix instance binding takes up four attributes.
#define GL_VERTEX_INSTANCE_MATRIX_ATTRIBUTE 3
#define GL_VERTEX_TANGENT_ATTRIBUTE 7
#define GL_VERTEX_BITANGENT_ATTRIBUTE 8

#define GL_WRITE_BUFFER 0x0C03
#define GL_READ_WRITE_BUFFER 0x0C04

#define POSITION_TEXTURE "_gBufferPosition"
#define NORMAL_TEXTURE "_gBufferNormal"
#define DEPTH_STENCIL_TEXTURE "_gBufferDepth"
#define COLOR_TEXTURE "_gBufferColor"
#define COMPOSITE_TEXTURE "_gBufferCompositeTexture"

