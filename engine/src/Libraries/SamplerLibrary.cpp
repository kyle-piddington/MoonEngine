#include <string>
#include "SamplerLibrary.h"
#include <memory>
using namespace MoonEngine;

SamplerLibrary::SamplerLibrary() {

}

SamplerLibrary::~SamplerLibrary()
{
    _samplers.clear();
}

/* Create or fetch a sampler, with integer and float params */
GLSampler * SamplerLibrary::getSampler(string samplerName,
        unordered_map<GLenum, GLint> paramsi,
        unordered_map<GLenum, GLfloat> paramsf) {
    if (_samplers.find(samplerName) == _samplers.end())
    {
        std::shared_ptr<GLSampler> glSampler = std::make_shared<GLSampler>();
        glSampler->init(paramsi, paramsf);
        _samplers[samplerName] = glSampler.get();
    }
    return _samplers[samplerName];
}

/* Create or fetch a sampler, with defaults if it does not exist */
GLSampler * SamplerLibrary::getSampler(string samplerName)
{
    return getSampler(samplerName, unordered_map<GLenum, GLint>(), unordered_map<GLenum, GLfloat>());
}

/* Create or fetch a sampler, with integer params */
GLSampler *SamplerLibrary::getSampler(std::string samplerName, unordered_map<GLenum, GLint> params) {
    return getSampler(samplerName, params, unordered_map<GLenum, GLfloat>());
}

