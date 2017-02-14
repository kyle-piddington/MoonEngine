#pragma once
#include <glfw/glfw3.h>
#include "util/Logger.h"
#include "GLWrapper/GLConstants.h"
using namespace  MoonEngine;

inline void LOG_GL(std::string file, int line) {
    GLenum error = glGetError();
    switch (error) {
    case GL_INVALID_ENUM:
        LOG(ERROR, "GL_INVALID_ENUM", file, line);
        break;
    case GL_INVALID_VALUE:
        LOG(ERROR, "GL_INVALID_VALUE", file, line);
        break;
    case GL_INVALID_OPERATION:
        LOG(ERROR, "GL_INVALID_OPERATION", file, line);
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        LOG(ERROR, "GL_INVALID_FRAMEBUFFER_OPERATION", file, line);
        break;
    case GL_OUT_OF_MEMORY:
        LOG(ERROR, "GL_OUT_OF_MEMORY", file, line);
        break;
    }
}