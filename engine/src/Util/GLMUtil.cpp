#include "GLMUtil.h"
#include <glm/gtc/type_ptr.hpp>

using namespace MoonEngine;

glm::vec3 GLMUtil::FromHex(unsigned hexCode)
{
    glm::vec3 ret;
    ret.x = (((hexCode & 0xFF0000) >> 16)) / 255.0f;
    ret.y = (((hexCode & 0x00FF00) >> 8)) / 255.0f;
    ret.z = ((hexCode & 0x0000FF)) / 255.0f;
    return ret;
}


glm::mat4 GLMUtil::FromAssimp(const aiMatrix4x4 & aiMat)
{
	
	glm::mat4 matrix = glm::mat4(aiMat.a1, aiMat.a2, aiMat.a3, aiMat.a4,
								 aiMat.b1, aiMat.b2, aiMat.b3, aiMat.b4,
								 aiMat.c1, aiMat.c2, aiMat.c3, aiMat.c4,
								 aiMat.d1, aiMat.d2, aiMat.d3, aiMat.d4);
	return glm::transpose(matrix);//.transpose();
	
}