#pragma once
/**
 * GLProgram links two or more shader objects together into 
 * a final program, and uses RAII principles to ensure 
 * only one program is created at a time.
 */
#include "OpenGL.h"
#include "GLShader.h"
#include <string>
#include <unordered_map>

namespace MoonEngine
{
    class GLProgram
    {
    public:

        /**
         * Create a named program object. (Useful for )
         */
        GLProgram(std::string name);

        /**
         * Delete a program object.
         */
        ~GLProgram();

        /**
         * Copy constructor(s) is disallowed for programs.
         */
        GLProgram(const GLProgram & other) = delete;

        GLProgram & operator=(const GLProgram &) = delete;

        /**
         Move Constructor(s)
         */
        GLProgram(GLProgram && other);

        GLProgram & operator=(GLProgram && other);

        /**
         * Attach a shader object to this program.
         * @param shader the shader to attach.
         */
        void attachShader(const GLShader & shader);

        /**
         * Complete and link the program.
         * @return true if no issues.
         */
        bool link();

        /**
         * Check to see if the program is ready to be bound.
         * @return true if the program is compiled and linked.
         */
        bool isReady() const;

        /**
         * Enable the program
         * Return true if program is enabled.
         */
        bool enable() const;

        /**
         * Query the program for a uniform name.
         * @param  uniformName the name of the uniform
         * @return             an integer if the uniform was found, or -1
         */
        GLint getUniformLocation(std::string uniformName);

        bool hasUniform(std::string uniformName);

        /**
         * Query the program for an attribute name
         * @param  attributeName the name of the attribute
         * @return               the name of the attribute.
         */
        GLint getAttributeLocation(std::string attributeName);

        std::string getName() const;

        /**
         * Get the handle to the program
         * @return the program handle
         */
        GLuint getObject() const;

    private:

        GLuint release();

        GLuint reset(GLuint otherID = 0);

        GLuint _progId;
        std::string _progName;
        bool _linked;
        std::unordered_map<std::string, GLint> _attribMap;
        std::unordered_map<std::string, GLint> _uniformMap;

    };
}