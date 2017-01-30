#pragma once
#include "OpenGL.h"

/**
 * RAII Style shader. When this
 * object is deconstructed, so is the shader source.
 */
namespace MoonEngine
{
	class GLShader
	{
	public:
		/**
		 * Create an empty shader.
		 * empty shaders cannot be compiled.
		 */
		GLShader();
		/**
		 * Construct and compile a shader.
		 */
		GLShader(GLenum shaderType, const char * source);

		/**
		 * Remove a shader.
		 */
		~GLShader();

		/**
		 * Copy constructor(s) is disallowed for Shaders.
		 */
		GLShader(const GLShader & other) = delete;
		GLShader &operator=(const GLShader &) = delete;

		/**
		 * Move Constructors
		 */
		GLShader(GLShader && other);
		GLShader &operator=(GLShader && other);
		/**
		 * Check the shader status to see if it's compiled
		 * @return true if compiled.
		 */
		bool isCompiled() const;

		/**
		 * Get the object ID of the shader
		 * @return true if compiled.
		 */
		GLuint getObject() const;

		/**
		 * Return the type of shader (Vertex/Fragment/etc)
		 * @return the shader type.
		 */
		GLenum getShaderType() const;

	
	private:
		bool compiled;
		GLuint _objectId;
		GLenum _shaderType;
		GLuint release();
		GLuint reset(GLuint newObject = 0);
	};
}