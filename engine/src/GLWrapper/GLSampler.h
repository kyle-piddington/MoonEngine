#pragma once

#include <unordered_map>
#include "OpenGL.h"

using namespace std;
namespace MoonEngine
{
    class GLSampler
    {
    public:

        /**
         * Create a sampler object.
         */
        GLSampler();

        /**
         * Delete a sampler object.
         */
        ~GLSampler();


        GLuint getId();

        void init(unordered_map<GLenum, GLint>, unordered_map<GLenum, GLfloat>);

    private:
        GLuint _samplerId;
    };
}