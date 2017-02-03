#include "GLMUtil.h"

using namespace MoonEngine;

glm::vec3 GLMUtil::FromHex(unsigned hexCode)
{
    glm::vec3 ret;
    ret.x = (((hexCode & 0xFF0000) >> 16)) / 255.0f;
    ret.y = (((hexCode & 0x00FF00) >> 8)) / 255.0f;
    ret.z = ((hexCode & 0x0000FF)) / 255.0f;
    return ret;
}

