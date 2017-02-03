#pragma once

#include "GLWrapper/GLShader.h"
#include "GLWrapper/GLProgram.h"

namespace MoonEngine
{
    namespace GLProgramUtilities
    {
        /**
         * Open and read from a file to load a shader
         * @param  fileName relative or absolute path to the file
         * @return          a shader object.
         */
        GLShader createShaderFromFile(GLenum type, std::string fileName);

        /**
         * Check the status of a shader, and pring
         * out an error if it's not compiled.
         * @param  shader The shader to check
         * @return        true if shader is ok.
         */
        bool checkShaderStatus(const GLShader & shader);

        /**
         * Check the status of a program, and print
         * out an error if it's not compiled.
         * @param  shader The program to check
         * @return        true if shader is ok.
         */
        bool checkProgramStatus(const GLProgram & program);
    }

}