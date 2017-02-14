#pragma once
/**
 * SamplerLibrary provides a mapping of
 * samplers to a name
 */
#include <string>
#include <unordered_map>
#include "GLWrapper/GLTexture.h"
#include <vector>
#include <memory>

namespace MoonEngine
{
    class SamplerLibrary
    {
    public:
        SamplerLibrary();

        ~SamplerLibrary();

        GLSampler * getSampler(std::string samplerName);

        GLSampler * getSampler(std::string samplerName, unordered_map<GLenum, GLint> params);

        GLSampler * getSampler(std::string samplerName,
             unordered_map<GLenum, GLint> paramsi,
             unordered_map<GLenum, GLfloat> paramsf);

    private:
        std::unordered_map<std::string, GLSampler *> _samplers;
        std::vector<std::shared_ptr<GLSampler>> _samplerVector;
    };
}