#include <stdlib.h>
#include <tuple>
#include <vector>
#include "GLSampler.h"

using namespace MoonEngine;

GLSampler::GLSampler() :
        _samplerId(0)
{

}

GLSampler::~GLSampler()
{

}

void GLSampler::init(unordered_map<GLenum, GLint> paramsi, unordered_map<GLenum, GLfloat> paramsf) {
    /* Set defaults if no parameters given */
    if (paramsi.empty() && paramsf.empty()) {
        paramsi = {
                /* Set wrap modes for the S and T directions */
                make_tuple(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE),
                make_tuple(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE),

                /* Set filtering mode for magnification and minification */
                make_tuple(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR),
                make_tuple(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        };

        paramsf = {
                make_tuple(GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f)
        };
    }
    glGenSamplers(1, &_samplerId);

    for (auto &param: paramsi) {
        glSamplerParameteri(_samplerId, get<0>(param), get<1>(param));
    }

    for (auto &param: paramsf) {
        glSamplerParameterf(_samplerId, get<0>(param), get<1>(param));
    }
}

GLuint GLSampler::getId() {
    return _samplerId;
}

